#include "sos_defines.h"
#include "couche1.h"
#include "couche3.h"
#include "couche2.h"
#include "couche4.h"
/*le nom du disque virtuel : variables globale */
virtual_disk_t virtual_disk_sos;

/*---------------------------------------------------------------------------------------------*/
void init_disk_sos(char *dirname){
   char filename[60];
   /*initialisation du superblock*/
   virtual_disk_sos.super_block.number_of_files = 0; //A incrumenter apres chaque ajout d'un file et décrumenter apres chauqe supression
   virtual_disk_sos.super_block.number_of_users = 1; //A incrumenter apres chaque ajout d'un user et décrumenter apres chauqe supression
   virtual_disk_sos.super_block.nb_blocks_used = NB_BLOCS ; // A incrumenter apres chaque appel à write_block
   virtual_disk_sos.super_block.first_free_byte = sizeof(struct super_block_s) + sizeof(struct inode_s)*INODE_TABLE_SIZE + sizeof(struct user_s)*NB_USERS + 60*sizeof(char);//octet juste après le super bloc et la table des inodes
   /*initialisation de la table des inodes*/
   for(int i=0; i< INODE_TABLE_SIZE ;i++){
     virtual_disk_sos.inodes[i].size = -1;
   }

   /*fichier vdisk du système de fichiers*/
   sprintf(filename,"%s/d0", dirname);
   strcpy(virtual_disk_sos.storage,filename); //nom du storage
}

/*---------------------------------------------------------------------------------------------*/
/*fonction pour sauvgarder le catalogue sur disque*/

/*---------------------------------------------------------------------------------------------*/
 uint compute_nblock(uint nb_octects){
   if(nb_octects % BLOCK_SIZE == 0){
     return nb_octects/BLOCK_SIZE;
   }
   else{
     return (nb_octects/BLOCK_SIZE) + 1;
   }
 }

/*---------------------------------------------------------------------------------------------*/
 int write_block(block_t block, long pos){
   /*ouverture du fichier disque*/
   FILE *d0 = fopen(virtual_disk_sos.storage, "a");
   assert(d0!=NULL);

   /*positionement dans le fichier*/
   int postionner = fseek(d0, pos, SEEK_SET);
   assert(postionner != -1);

   /*ecriture du block */
   size_t nbInfos_ecris= fwrite(&(block.data), BLOCK_SIZE, 1,d0);
   if(nbInfos_ecris != 1){
     perror("erreur ecriture block");
     return -1;
   }
   virtual_disk_sos.super_block.nb_blocks_used+=1;
   fclose(d0);
   return 0;
 }

/*---------------------------------------------------------------------------------------------*/
int read_block(block_t *block,long pos){
  /*ouverture du fichier disque*/
  FILE *d0 = fopen(virtual_disk_sos.storage, "r");
  assert(d0!=NULL);

  /*positionement dans le fichier*/
  int postionner = fseek(d0,pos, SEEK_SET);
  assert(postionner != -1);
  /*lecture du block*/
  int nbInfos_lus=fread(&(block->data),BLOCK_SIZE, 1,d0);
  if(nbInfos_lus != 1){
    perror("erreur lecture block");
    return -1;
  }
  fclose(d0);
  return 0;
}

/*---------------------------------------------------------------------------------------------*/
void afficher_block(long pos){
  block_t block;
  /*on fait appel à la fonction read block*/
  int lecture = read_block(&block,pos);
  if(lecture != 0){
    perror("erreur read_block");
    return;
  }
  printf("%s",block.data);
}
/*---------------------------------------------------------------------------------------------*/

void verify_upload(){
  //inode_table_t inodes;
  //int nb_inodes;
  printf("\n--------------------------------------ScratchOS---------------------------\n");

  /*verify le nom du disque*/
  printf("\n---------------------------------------Storage----------------------------\n");
  printf("%s\n",virtual_disk_sos.storage);
  /*verify user table : it contains just the root*/
  read_user_table();//lecture user_table dans d0 (l'info est sauvgarder en virtual_disk_sos.user_table)
  printf("-----------------------------------------Users------------------------------\n");
  for(int i=0; i < virtual_disk_sos.super_block.number_of_users; i++){
    printf("user %d: %s\n", i+1, virtual_disk_sos.users_table[i].login);
  }

  /*verify superbloc*/
  read_super_block();//lecture super_block dans d0 (l'info est sauvgarder en virtual_disk_sos.super_block)
  printf("\n---------------------------------------Super bloc-------------------------\n");
  printf("number of files: %d\n",virtual_disk_sos.super_block.number_of_files);
  printf("number of users: %d\n",virtual_disk_sos.super_block.number_of_users);
  printf("number of blocs used: %d\n",virtual_disk_sos.super_block.nb_blocks_used);
  printf("first free byte: %d\n",virtual_disk_sos.super_block.first_free_byte);

  /*verify inode_table*/
  read_inodes_table();//lecture inodes_table dans d0 (l'info est sauvgarder en virtual_disk_sos.inodes_table)
  printf("\n------------------------------------Table d'inodes------------------------\n");
  for(int i=0; i < virtual_disk_sos.super_block.number_of_files; i++){
    printf("-----inode[%d]\n",i);
    printf("filename: %s\nsize: %d\n",virtual_disk_sos.inodes[i].filename, virtual_disk_sos.inodes[i].size);
    printf("uid: %d\ndate creation: %s\n",virtual_disk_sos.inodes[i].uid, virtual_disk_sos.inodes[i].ctimestamp);
    printf("date modification: %s\nnblock:%d\n",virtual_disk_sos.inodes[i].mtimestamp, virtual_disk_sos.inodes[i].nblock);
  }
  /*verify user table*/
  printf("\n-----------------------------------Table users----------------------------\n");
  for(int i=0; i < virtual_disk_sos.super_block.number_of_users; i++){
    printf("--- user: %d\n",i+1);
    printf("login: %s\n",virtual_disk_sos.users_table[i].login);
    printf("passWord: %s\n",virtual_disk_sos.users_table[i].passwd);
  }
  printf("\n------------------------verification contenu fichier passwd---------------\n");
  file_t file;

  read_file("passwd",&file);
  for(int i=0; i <virtual_disk_sos.inodes[0].nblock ; i++){
    afficher_block((long)(virtual_disk_sos.inodes[0].first_byte +i*BLOCK_SIZE));
  }
  printf("\n");
}
