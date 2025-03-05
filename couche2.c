#include "sos_defines.h"
#include "couche2.h"
#include "couche1.h"
#include "couche3.h"
#include "timestamp.h"

char *timestamp(){
    time_t current_time;
    char* c_time_string;

    /* Obtain current time. */
    current_time = time(NULL);

    if (current_time == ((time_t)-1))
    {
        (void) fprintf(stderr, "Failure to obtain the current time.\n");
        exit(EXIT_FAILURE);
    }

    /* Convert to local time format. */
    c_time_string = ctime(&current_time);

    if (c_time_string == NULL)
    {
        (void) fprintf(stderr, "Failure to convert the current time.\n");
        exit(EXIT_FAILURE);
    }
    c_time_string[strlen(c_time_string)-1] = '\0';
    return c_time_string;
}
/*---------------------------------------------------------------------------------------------*/

void write_storage(){
   /*on enregistre dans le disque virtuel le nom du storage */
   FILE *d0 = fopen(virtual_disk_sos.storage, "a");
   assert(d0!=NULL);

   /*positionement dans le fichier*/
   int postionner = fseek(d0, sizeof(struct super_block_s) + sizeof(struct inode_s)*INODE_TABLE_SIZE + sizeof(struct user_s)*NB_USERS, SEEK_SET);
   assert(postionner != -1);

   /*ecriture */
   size_t nbInfos_ecris= fwrite(virtual_disk_sos.storage, 60*sizeof(char), 1,d0);
   if(nbInfos_ecris != 1){
     perror("erreur ecriture storage");
   }
   fclose(d0);
}

/*---------------------------------------------------------------------------------------------*/

void read_storage(){
   /*on recuperer le nom du storage du disque virtuel */
   FILE *d0 = fopen("d0", "r");
   assert(d0!=NULL);

   /*positionement dans le fichier*/
   int postionner = fseek(d0, sizeof(struct super_block_s) + sizeof(struct inode_s)*INODE_TABLE_SIZE + sizeof(struct user_s)*NB_USERS, SEEK_SET);
   assert(postionner != -1);

   /*ecriture */
   size_t nbInfos_Lus= fread(virtual_disk_sos.storage, 60*sizeof(char), 1,d0);
   if(nbInfos_Lus != 1){
     perror("erreur lecture storage");
   }
   fclose(d0);
}

/*------------------------------------------------------------------------------------------------------------*/
int write_super_block(void){
    /*ouverture du fichier disque*/
    FILE *d0 = fopen(virtual_disk_sos.storage, "w");
    assert(d0!=NULL);

    /*ecriture du super bloc  à l'adresse 0 debut du disque*/
    int postionner = fseek(d0, 0, SEEK_SET);
    assert(postionner != -1);

    size_t nbInfos_ecris = fwrite(&(virtual_disk_sos.super_block), sizeof(struct super_block_s), 1, d0);
    if(nbInfos_ecris != 1){
      perror("erreur ecriture superbloc");
      return -1;
    }

    fclose(d0);
    return 1;
}

/*------------------------------------------------------------------------------------------------------------*/

int read_super_block(void){
    /*ouverture du fichier disque*/
    FILE *d0 = fopen(virtual_disk_sos.storage, "r"); // ouverture en lecture et écriture
    assert(d0!=NULL);

    /*lecture du super bloc  à l'adresse 0*/
    int postionner = fseek(d0, 0, SEEK_SET);
    assert(postionner != -1);

    size_t nbInfos_lus = fread(&(virtual_disk_sos.super_block), sizeof(struct super_block_s), 1, d0);
    if(nbInfos_lus != 1){
      perror("erreur lecture superbloc");
      return -1;
    }
    fclose(d0);
    return 1;
}
/*------------------------------------------------------------------------------------------------------------*/

void update_first_free_byte(int nb_blocs_ecris){
  virtual_disk_sos.super_block.first_free_byte += (uint) nb_blocs_ecris * BLOCK_SIZE;
}
/*------------------------------------------------------------------------------------------------------------*/
int read_inodes_table(void){
  /*ouverture du fichier disque*/
  FILE *d0 = fopen(virtual_disk_sos.storage, "r");
  assert(d0!=NULL);

  /*se positionner apres le superbloc*/
  int postionner = fseek(d0, sizeof(struct super_block_s), SEEK_SET);
  assert(postionner != -1);
  /*charger la table des inodes depuis le systeme*/

  size_t nbInfos_lus = fread(&(virtual_disk_sos.inodes), sizeof(struct inode_s)*INODE_TABLE_SIZE, 1, d0);
  if(nbInfos_lus != 1){
    perror("erreur lecture inode\n");
    return -1;
  }

  fclose(d0);
  return 1;
}
/*------------------------------------------------------------------------------------------------------------*/
int write_inodes_table(void){
  /*ouverture du fichier disque*/
  FILE *d0 = fopen(virtual_disk_sos.storage, "a");
  assert(d0!=NULL);

  /*se positionner apres le superbloc*/
  int postionner = fseek(d0, sizeof(struct super_block_s), SEEK_SET);
  assert(postionner != -1);
  /*charger la table des inodes depuis le systeme*/

  size_t nbInfos_lus = fwrite(&(virtual_disk_sos.inodes), sizeof(struct inode_s)*INODE_TABLE_SIZE, 1, d0);
  if(nbInfos_lus != 1){
    perror("erreur ecriture inode\n");
    return -1;
  }
  fclose(d0);
  return 1;
}

/*------------------------------------------------------------------------------------------------------------*/
void delete_inode(int indice_inode){
  /*suppression de l'inode en le remplaçant par un vide*/
  inode_t inode_vide;
  inode_vide.size=-1;
  int i;

  /*decalage de la table*/
  for(i=indice_inode; i < INODE_TABLE_SIZE-1; i++){
    virtual_disk_sos.inodes[i]=virtual_disk_sos.inodes[i+1];
  }
  virtual_disk_sos.inodes[i]=inode_vide;
}

/*------------------------------------------------------------------------------------------------------------*/
int get_unused_inode(void){
  int i=0;
  while(i<INODE_TABLE_SIZE){
    if(virtual_disk_sos.inodes[i].size== -1){
      return i;
    }
    i++;
  }
  return -1;
}

/*------------------------------------------------------------------------------------------------------------*/

void init_inode(inode_t *inode, char File_name[],uint taille,int pos){
  strcpy(inode->filename,File_name);
  inode->size=taille;
  if(strcmp(File_name , "passwd")==0){
    inode->uid=0;
  }
  inode->uid=session.userid; //l'utlisateur createur
  inode->uright=RW; //RW droit lecture et ecriture par defaut
  inode->oright=rw; // rw aucun droit par defaut
  strcpy(inode->ctimestamp,timestamp());
  strcpy(inode->mtimestamp,timestamp());
  inode->nblock=compute_nblock(taille);
  inode->first_byte=pos;
}

/*------------------------------------------------------------------------------------------------------------*/

int update_disk(char *tem_file){
  int result_write, result;

  FILE *temp = fopen(tem_file, "w+");
  assert(temp != NULL);

  FILE *d0 = fopen(virtual_disk_sos.storage, "r+");
  assert(d0 != NULL);

  ///////// On copie les infos de d0 apartir de la opsition 1641 dans temp a la opsition 0 ///////////
  //on se possitionne apres les storage (size = 1641)
  int postionner = fseek(d0, virtual_disk_sos.inodes[0].first_byte, SEEK_SET);
  assert(postionner != -1);
  postionner = fseek(temp, 0, SEEK_SET);
  assert(postionner != -1);

  block_t block_i;

  while((result = fread(&(block_i), BLOCK_SIZE, 1, d0)) > 0){
    result_write = fwrite(&(block_i), BLOCK_SIZE, 1, temp);
  }

  ///////// Ca efface les infos du disque d0 et on write superBlock, inodes_table, user_table et storage ///////////
  result_write = write_super_block();
  if(result_write != 1){
    printf("Writing superBlock failed :(\n");
    return -1;
  }

  result_write = write_inodes_table();
  if(result_write != 1){
    printf("Writing inodes table failed :(\n");
    return -1;
  }

  result_write = write_user_table();
  if(result_write != 1){
    printf("Writing users table failed :(\n");
    return -1;
  }

  write_storage();

  ///////// write superBlock, inodes_table, user_table et storage ///////////
  //on se possitionne apres les storage (size = 1641)
  postionner = fseek(d0, virtual_disk_sos.inodes[0].first_byte, SEEK_SET);
  assert(postionner != -1);
  postionner = fseek(temp, 0, SEEK_SET);
  assert(postionner != -1);

  block_t block_j;

  while((result = fread(&(block_j), BLOCK_SIZE, 1, temp)) > 0){
    result = fwrite(&(block_j), BLOCK_SIZE, 1, d0);
    if(result_write != 1){
      return -1;
    }
  }

  fclose(d0);
  fclose(temp);
  return 1;
}
