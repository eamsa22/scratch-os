#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sos_defines.h"
#include "couche4.h"
#include "couche1.h"
#include "couche2.h"

/*------------------------------------------------------------------------------------------------------------*/
int search_inode(char *File_name){
  int i=0;
  while(i<INODE_TABLE_SIZE ){
    if(strcmp(virtual_disk_sos.inodes[i].filename,File_name) == 0 ){
      return i;
    }
    i++;
  }
  return -1;
}
/*------------------------------------------------------------------------------------------------------------*/

int write_file(char *File_name ,file_t file){
  int i_inode, delete, i;
  block_t block;
  uint nb_blocs;

  if (file.size>MAX_FILE_SIZE){
    // Vérification que l'on peut écrire dans le fichier
    printf("stderr : Oups, ton fichier est trop grand pour etre écris dans le disque :'( \n");
    printf("stderr : Ecris  un fichier de taille inférieures à %d \n", MAX_FILE_SIZE );
    return 0;
  }

  if (strlen(File_name)> FILENAME_MAX_SIZE){
    printf("stderr : Oups, ton nom de fichier est trop grand, peux tu écrire un nom de fichier dont la taille est inférieure à %d caractères ? Merci de ta compréhension \n", FILENAME_MAX_SIZE );
    return 0;
  }
  /*si le fichier existe et taille file <= taille ancienne file*/
  if((i_inode=search_inode(File_name)) != -1  && (virtual_disk_sos.inodes[i_inode].size) <= file.size ){
    virtual_disk_sos.inodes[i_inode].size = file.size;
    virtual_disk_sos.inodes[i_inode].nblock = compute_nblock(file.size);
    strcpy(virtual_disk_sos.inodes[i_inode].mtimestamp, timestamp());
    /*modif data disk*/
    i=0;
    while(i < virtual_disk_sos.inodes[i_inode].nblock){
      strncpy((char*)block.data, (char*)file.data + (i*BLOCK_SIZE), (i*BLOCK_SIZE)+BLOCK_SIZE);
      if(write_block(block,(virtual_disk_sos.inodes[i_inode].first_byte)+i*BLOCK_SIZE)==ERREUR_ECRITURE){
        printf("ecriture fichier échouée\n");
        return 0;
      }
      i++;
    }
    int result=update_disk("tem_file");//temp_file == file temporaire
    if(result != 1){
      printf("error update_disk\n");
    }
    return 1;
  }
  else{
    if(i_inode != -1  && virtual_disk_sos.inodes[i_inode].size > file.size){
      delete=delete_file(File_name) /*supprime l'inode et les donnees sur le disque*/;
      if(delete ==0){
        return 0;
      }
    }

    i_inode=get_unused_inode();
    if(i_inode == -1){
      printf("la table des inodes est pleins! impossible d'ajouter un nouveau fichier\n");
      return 0;
    }
    /*initialisation de l'inode à la fin du table*/
    init_inode(&(virtual_disk_sos.inodes[i_inode]),File_name ,file.size,virtual_disk_sos.super_block.first_free_byte);
    /*ecriture du fichier sur le disque*/
    nb_blocs = compute_nblock(file.size);
    i=0;
    while(i<nb_blocs){
      strncpy((char*)block.data,(char*)file.data+(i*BLOCK_SIZE),(size_t) BLOCK_SIZE);
      //bloc.data[i]=file.data[i]
      if(write_block(block,(virtual_disk_sos.super_block.first_free_byte)+i*BLOCK_SIZE)==-1){
        printf("ecriture fichier échouée\n");
        return 0;
      }
      i++;
    }
    update_first_free_byte(nb_blocs);
    virtual_disk_sos.super_block.number_of_files+=1;
    int result=update_disk("tem_file");//temp_file == file temporaire
    if(result != 1){
      printf("error update_disk\n");
    }
    return 1;
  }
  return 0;
}
/*------------------------------------------------------------------------------------------------------------*/
int read_file(char *File_name , file_t *file){
  int i=0;
  int i_inode;
  block_t block;
  uint nb_blocs;

  if((i_inode=search_inode(File_name)) != -1){
    nb_blocs=virtual_disk_sos.inodes[i_inode].nblock;
    while(i<nb_blocs){
      if(read_block(&block,(virtual_disk_sos.inodes[i_inode].first_byte+i*BLOCK_SIZE))==ERREUR_LECTURE){
        printf("lecture fichier échouée\n");
        return 0;
      }
      strcpy((char*)file->data+(i*BLOCK_SIZE),(char*)block.data);
      i++;
    }
    file->size=virtual_disk_sos.inodes[i_inode].size;
    return 1;
  }
  return 0;
}
/*------------------------------------------------------------------------------------------------------------*/
int delete_file(char *File_name){
  int i=0;
  int i_inode;
  block_t block;

  if((i_inode=search_inode(File_name)) != -1){
    /*supression du contenu du fichier du disque laisser un trou assimilé par des espaces*/
    while(i<virtual_disk_sos.inodes[i_inode].nblock){
      strcpy((char*)block.data,"   ");
      if(write_block(block,(virtual_disk_sos.super_block.first_free_byte)+i*BLOCK_SIZE)==-1){
        printf("erreur supression file\n");
        return 0;
      }
      i++;
    }
    /*supression de l'inode de la table des inodes*/
    delete_inode(i_inode);
    return 1;
  }
  return 0;
}
/*------------------------------------------------------------------------------------------------------------*/
int load_file_from_host(char *File_name){
  FILE *file;
  file_t fichier;
  size_t nbInfos_lus;
  int ecriture ,i;
  /*ouverture de fichier*/
  file = fopen(File_name, "r");
  if(file == NULL){
    fprintf(stderr, "erreur lors de l'ouverture du fichier !\n");
    return 0;
  }
  /*lecture des données dans une structure de type file_t*/
  fichier.size=0;
  i=0;
  while((nbInfos_lus=fread(&(fichier.data)+(i*BLOCK_SIZE),BLOCK_SIZE, 1,file)) != 0){
    fichier.size+=nbInfos_lus;
    i++;
  }
  /*ecriture du fichier sur le disque*/
  fclose(file);
  ecriture = write_file(File_name ,fichier);
  return ecriture;
}
/*------------------------------------------------------------------------------------------------------------*/
int store_file_to_host(char *File_name){
  FILE *file;
  file_t fichier;
  int reading;
  size_t nbInfos_ecris;
  int code_retour=1;
  if((reading = read_file(File_name,&fichier)) == 0){
    return 0;
  }
  /*creation / ouverture de fichier*/
  file = fopen(File_name, "w");
  if(file == NULL){
    fprintf(stderr, "erreur lors de l'ouverture du fichier !\n");
    return 0;
  }
  /*ecriture du fichier sur le systeme host*/
  nbInfos_ecris= fwrite(&(fichier.data),fichier.size, 1,file);
  if(nbInfos_ecris != 1){
    code_retour=0;
  }
  fclose(file);
  return code_retour;
}

/*-----------------------------------------------------------------------------------------------------------------*/

int defragmentateur(){
  //On écrit uniquement la section bloc de donnée dans le fichier temporaire, la section "inode" sera uniquement mise à jour.
  FILE * fd=fopen(virtual_disk_sos.storage,"r+");
  FILE * temp=fopen("temporaire", "w+");
  if ((fd == NULL) || (temp==NULL)){
    printf("stderr: Erreur ouverture fichier lors de la défragmentation");
    return 0;
  }
  int nombre_inode=read_inodes_table();
  char Memoire_tampon[MAX_FILE_SIZE+1];
  int Nb_Total=0;
  size_t Lu;
  int Ecris;
  int ecart;
  //off_t position;
  //Pour mettre à jour le disque, on va créer un fichier, et écrire en faisant
  //Dans toute la table d'inode, rechercher les first_byte , le soustraire au first_byte suivant, et on a 2 cas:
  //1. Si c'est supérieur à size, alors il y'a des espaces à effacer, puis tout les first_byte suivants doivent etre mis à jour
  //2. Si c'est égal, on passe au noeud suivant :)
  //D'abord, on va écrire dans le fihcier temporaire jusqu'à arriver aux blocs de données de texte

//Maintenant, on commence à, agir sur les blocs de données
  for(int i=0;i<nombre_inode-1;i++){
    //Ecriture dans le fichier temporaire les blocs de données brutes
    if (!((virtual_disk_sos.inodes[i+1].first_byte)-(virtual_disk_sos.inodes[i].first_byte)==virtual_disk_sos.inodes[i].size)){
    //ecart== Vérification que le ficheir fait bien la taille qu'il est censé faire et qu'un fichier n'a pas été supprimé dans la table des inodes jsute après lui
      ecart=((virtual_disk_sos.inodes[i+1].first_byte)-(virtual_disk_sos.inodes[i].first_byte))-virtual_disk_sos.inodes[i].size;

      //Ici du coup on écrit le contenu du fichier dnas le fichier temp en s'arretant avant les espaces, puis on passera à la tbale d'inode suivante et on mettra ) jour la table d'inodes
      fseek(fd,virtual_disk_sos.inodes[i].first_byte+Nb_Total,SEEK_SET);
      //Lu=read(fd,Memoire_tampon,virtual_disk_sos.inodes[i].size-ecart);
      Lu=fread(Memoire_tampon,1,virtual_disk_sos.inodes[i].size-ecart,fd);
      //Ecris=write(temp,Memoire_tampon,Lu);
      Ecris=fwrite(Memoire_tampon,1,Lu,temp);
      assert(Ecris != 1);
      //Vérifie que le nombre d'octet écris par rapport à la taille est bon.
      ecart=0;
      // Mise à jour de tout les champs first byte de la table d'inode
      for(int m=i;m<nombre_inode-1;m++){
        virtual_disk_sos.inodes[m].first_byte=virtual_disk_sos.inodes[m].first_byte - ecart;

        //Fait gaffe à ce que tu ne modifie pas tout tes first_byte
      }
      Nb_Total=Nb_Total+ecart;
      ecart=0;
    }
    else{
      //Ici c'est le cas où le fichier n'a pas d'espace en trop, du coup on écrit juste normalement
      fseek(fd,virtual_disk_sos.inodes[i].first_byte+Nb_Total,SEEK_SET);
      //Lu=read(fd,Memoire_tampon,virtual_disk_sos.inodes[i].size);
      Lu=fread(Memoire_tampon,1,virtual_disk_sos.inodes[i].size,fd);

      //Ecris=write(temp,Memoire_tampon,Lu);
      Ecris=fwrite(Memoire_tampon,1,Lu,temp);
      assert(Ecris != 1);

    }
  }
  //Tout a été écriss avec succès dans le fichier temporaire

  printf("Le défragmentage a été réalisé avec succès :)");

  //Maintenant, on va fermer fd pour tout réécrire, en rajoutant le fichier temporaire à la fin.

/*  fclose(fd);
  write_super_block();
  write_inodes_table();
  write_user_table();
  write_storage();
  //the real deal
  fopen(virtual_disk_sos.storage,"a+");
  //Puis au final, on écrit les nblocs de données du fichier temp qui n'a plus les espaces en trop dans le fichier original à partir de first_byte de l'inode[0]
  fseek(fd,virtual_disk_sos.inodes[0].first_byte,SEEK_SET);
  //Lu=read(temp,Memoire_tampon,MAX_FILE_SIZE
  while(Lu=fread(Memoire_tampon,1,MAX_FILE_SIZE,temp)>0){
    fwrite(Memoire_tampon,1,Lu,fd);
    //write(fd,Memoire_tampon,Lu);
  }*/
  update_disk("temporaire");
  fclose(temp);
  //
  return 1;
}
