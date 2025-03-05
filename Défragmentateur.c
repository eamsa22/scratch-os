#include "sos_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include "couche1.h"
#include "couche2.h"



//On écrit uniquement la section bloc de donnée dans le 2ème fichier, la section "inode" sera uniquement mise à jour.
int defragmentateur(){
  FILE * fd=fopen(virtual_disk_sos.storage,"r+");
  FILE * temp=fopen("temporaire", "w+");
  if ((fd == NULL) || (temp==NULL)){
    printf("stderr: Erreur ouverture fichier lors de la défragmentation");
    return 0;
  }
  int nombre_inode=read_inodes_table();
  char Memoire_tampon[MAX_FILE_SIZE+1];
  int Nb_Lus, Nb_Total=0;
  size_t Lu,Ecris;
  int ecart;
  off_t position;
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
  update("temporaire");
  fclose(temp);
  //
  return 1;
}
