#include<stdio.h>
#include <string.h>

#include "sos_defines.h"
#include "couche1.h"
#include "couche2.h"

int main(){
  /*creation of file*/
  FILE *file=fopen("d0","r");
  if(file == NULL){
    return -1;
  }
  /*ecreation de la table des inodes */
  inode_table_t inodes;
  char fileN[30];
  int size=70;
  for(int i=0; i < INODE_TABLE_SIZE;i++){
    sprintf(fileN,"test/%d",i);
    size+=i;
    init_inode(&(inodes[i]), fileN,size, i*INODE_SIZE*BLOCK_SIZE);
  }

  printf("\n--------test de l'initialisation d'inode----------\n");
  for(int i=0; i < INODE_TABLE_SIZE;i++){
    printf("filename: %s | size: %d | uright : %d | oright: %d |\n",inodes[i].filename,inodes[i].size,inodes[i].uright,inodes[i].oright);
    printf(" ctimestamp: %s | mtimestamp: %s |nblock: %d | first_byte: %d\n ",inodes[i].ctimestamp,inodes[i].mtimestamp,inodes[i].nblock,inodes[i].first_byte);
  }

  /*ecriture de l'inode sur le fichier*/
  printf("pos courante before write %ld\n",ftell(file));
  size_t nb_ecris;
  for(int i=0; i< INODE_TABLE_SIZE;i++){
    nb_ecris=fwrite(&(inodes[i]), INODE_SIZE*BLOCK_SIZE, 1, file);
    if(nb_ecris != 1){
      printf("ERREUR_ECRITURE inode \n");
      return -1;
    }
  }
  printf("pos courante after write %ld\n",ftell(file));

  /*lecture de l'inode a partir de file*/
  inode_table_t readInode;
  int pos=fseek(file, 0, SEEK_SET);
  if(pos == -1){
    printf("erreur positionnement au debut du fichier\n");
    return -1;
  }
  printf("pos courante before read %ld\n",ftell(file));
  size_t nb_lus;
  for(int i=0; i< INODE_TABLE_SIZE;i++){
    nb_lus=fread(&(readInode[i]), INODE_SIZE*BLOCK_SIZE, 1, file);
    if(nb_lus != 1){
      printf("ERREUR_lecture inode \n");
      return -1;
    }
  }
  printf("pos courante after read %ld\n",ftell(file));

  printf("\n--------test de lecture d'inode----------\n");
  for(int i=0; i < INODE_TABLE_SIZE;i++){
    printf("filename: %s | size: %d | uright : %d | oright: %d |\n",readInode[i].filename,readInode[i].size,readInode[i].uright,readInode[i].oright);
    printf(" ctimestamp: %s | mtimestamp: %s |nblock: %d | first_byte: %d\n ",readInode[i].ctimestamp,readInode[i].mtimestamp,readInode[i].nblock,readInode[i].first_byte);
  }
  fclose(file);
  return 0;
}
