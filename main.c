#include<stdio.h>
#include "sos_defines.h"
#include "couche1.h"
#include "couche2.h"
#include "couche3.h"
#include "couche4.h"
#include "couche5.h"
#include "commandes.h"

int main(){
  char dirname[100];
  file_t file;
  int result;

  //initialiser vdisk
  printf("saisir le  nom du rep :");
  scanf("%s",dirname);

  init_disk_sos(dirname); //init super block et table d'inode
  //session->userid=0;
  init_users_table(); // init table d'users


  result= write_super_block();
  if(result == -1){
    printf("writing superbloc failed :(\n");
  }

  result= write_inodes_table();
  if(result == -1){
    printf("writing inodes Table failed :(\n");
  }

  result= write_user_table();
  if(result == -1){
    printf("writing users table failed :(\n");
  }

  write_storage();

  /*creation fichier psswd*/
  file.size = sizeof(virtual_disk_sos.users_table[0].passwd) + sizeof("root ");
  strcpy((char*)file.data, "root");
  strcat((char*)file.data, " ");
  strcat((char*)file.data, virtual_disk_sos.users_table[0].passwd);

  result = write_file("passwd", file);
  if(result == 0){
    printf("writing passwd file failed :(\n");
  }

  result=update_disk("tem_file");//temp_file == file temporaire
  if(result != 1){
    printf("error update_disk\n");
  }
  verify_upload();

  return 0;
}
