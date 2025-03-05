#include "sos_defines.h"
#include "couche1.h"
#include "couche2.h"
#include "couche3.h"
#include "couche4.h"
#include "couche5.h"
#include "commandes.h"

int main(){
  printf("\n--------------ScratchOS--------------\n");
  printf("--------------------by Scratchy Group\n");
  read_storage();
  read_super_block();
  read_inodes_table();
  read_user_table();
  read_storage();
  //verify_upload();
  int result=lancer_interprete_cmd();
  if(result ==-1){
    printf("erreur lancement interpreteur de commandes\n");
  }
  update_disk("tem_file");
  return 0;
}
