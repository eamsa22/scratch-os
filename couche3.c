#include "sos_defines.h"
#include "couche3.h"
#include "couche2.h"
#include "couche1.h"

/*----------------------------------------------------------------------------------------*/

void init_users_table(){
  //BYTE passWord;
  char *item = malloc(30*sizeof(char));
  strcpy(virtual_disk_sos.users_table[0].login, "root");
  printf("saisissez un password : ");
  scanf("%s",item);
  /*calculer le haché du password*/
  sha256ofString((BYTE *)item, virtual_disk_sos.users_table[0].passwd);
}

/*----------------------------------------------------------------------------------------*/

int read_user_table(){
  /*ouverture du fichier disque*/
  FILE *d0 = fopen(virtual_disk_sos.storage, "r");
  assert(d0!=NULL);

  /*se positionner apres la table d'inodes*/
  int postionner = fseek(d0, sizeof(struct super_block_s) + sizeof(struct inode_s)*INODE_TABLE_SIZE, SEEK_SET);
  assert(postionner != -1);

  /*charger la table des users*/
  size_t nbInfos_lus = fread(&(virtual_disk_sos.users_table), sizeof(struct user_s)*NB_USERS, 1, d0);
  if(nbInfos_lus != 1){
    perror("erreur lecture inode\n");
    return -1;
  }
  return nbInfos_lus;
}

/*----------------------------------------------------------------------------------------*/

int write_user_table(){
  /*ouverture du fichier disque*/
  FILE *d0 = fopen(virtual_disk_sos.storage, "a");
  assert(d0!=NULL);

  /*se positionner apres le superbloc*/
  int postionner = fseek(d0, sizeof(struct super_block_s) + sizeof(struct inode_s)*INODE_TABLE_SIZE, SEEK_SET);
  assert(postionner != -1);
  /*charger la table des inodes depuis le systeme*/

  size_t nbInfos_lus = fwrite(&(virtual_disk_sos.users_table), sizeof(struct user_s)*NB_USERS, 1, d0);
  if(nbInfos_lus != 1){
    perror("erreur ecriture inode\n");
    return -1;
  }
  fclose(d0);
  return nbInfos_lus;
}

/*----------------------------------------------------------------------------------------*/

int add_user(user_t user){
  int indice_user=virtual_disk_sos.super_block.number_of_users;
  if(indice_user < NB_USERS){
      virtual_disk_sos.users_table[indice_user]=user;
      virtual_disk_sos.super_block.number_of_users+=1;
      return 0;
  }
  int result=update_disk("tem_file");//temp_file == file temporaire
  if(result != 1){
    printf("error update_disk\n");
  }
  return -1;
}

/*----------------------------------------------------------------------------------------*/

int remove_user(char *login){
  int i=1;
  user_t user_vide;
  strcpy(user_vide.login,"");
  /*chercher l'utilisateur a supprimer*/
  while(i<NB_USERS){
    if(strcmp(virtual_disk_sos.users_table[i].login, login) == 0){
      /*supprimer en l remplaçant avec une structure vide user_t puis compacter la table*/
      while(i < NB_USERS-1){
        virtual_disk_sos.users_table[i] = virtual_disk_sos.users_table[i+1];
        i++;
      }
      virtual_disk_sos.users_table[i] = user_vide;
      virtual_disk_sos.super_block.number_of_users-=1;
      return 0;
    }
    i++;
  }
  /*si non trouvé on renvoie -1*/
  return -1;
}
