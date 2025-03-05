#include "sos_defines.h"
#include "couche2.h"
#include "couche3.h"
#include "couche4.h"
#include "couche5.h"
#include "commandes.h"


/*-------------------------------------------------------------------------------------------------*/
char *getRight(uint right){
  char *r="";
  if(right == rw){
    r="rw";
  }
  else if(right==rW){
    r="rW";
  }
  else if(right==Rw){
    r="Rw";
  }
  else{
    r="RW";
  }
  return r;
}
/*-------------------------------------------------------------------------------------------------*/
uint getBinaryRight(char *right){
  uint r;
  if(strcmp(right ,"rw")== 0){
    r=rw;
  }
  else if(strcmp(right, "rW")==0){
    r=rW;
  }
  else if(strcmp(right,"Rw")==0){
    r=Rw;
  }
  else if(strcmp(right,"RW")==0){
    r=RW;
  }
  else{
    r=4;
  }
  return r;
}
/*-------------------------------------------------------------------------------------------------*/
uint get_user_right(int indice_inode){
  if(virtual_disk_sos.inodes[indice_inode].uid == session.userid){
    return virtual_disk_sos.inodes[indice_inode].uright;
  }
  else{
    return virtual_disk_sos.inodes[indice_inode].oright;
  }
}
/*-------------------------------------------------------------------------------------------------*/

int exec_ls(cmd_t cmd){
  if(cmd.nbArgs == 2){
    if(strcmp(cmd.tabArgs[1],"-l")==0){
      for(int i=0 ; i< virtual_disk_sos.super_block.number_of_files ;i++){
        char *user=virtual_disk_sos.users_table[virtual_disk_sos.inodes[i].uid].login;
        char *ownerR=getRight(virtual_disk_sos.inodes[i].uright);
        char *otherR=getRight(virtual_disk_sos.inodes[i].oright);
        printf("-%s-%s %s %d %s %s %s\n",ownerR,otherR,user,virtual_disk_sos.inodes[i].size,virtual_disk_sos.inodes[i].ctimestamp,virtual_disk_sos.inodes[i].mtimestamp,virtual_disk_sos.inodes[i].filename);
      }
    }
    else{
      printf("erreur option\n");
      return -1;
    }
  }
  else{
    for(int i=0; i< virtual_disk_sos.super_block.number_of_files;i++){
      printf("%s - %d octects\n",virtual_disk_sos.inodes[i].filename,virtual_disk_sos.inodes[i].size);
    }
  }
  return 0;
}
/*-------------------------------------------------------------------------------------------------*/

/*fonction pour executer la commande cat retourne 0 c'est execution bien passée*/
int exec_cat(cmd_t cmd){
  file_t file;
  int lecture , indice_inode;
  uint right;
  if(cmd.nbArgs != 2){
    printf("erreur nombre parametres cat\n");
    return -1;
  }
  else{
    /*verifier les droits de lecture*/
    indice_inode=search_inode(cmd.tabArgs[1]);
    if(indice_inode != -1){
      right=get_user_right(indice_inode);
      if(right == Rw || right== RW){
        /*lire le contenu du fichier*/
         if((lecture = read_file(cmd.tabArgs[1] ,&file)) == 1){
           /*affichage du contenu de fichier*/
           printf("%s\n",file.data);
           return 0;
         }
      }
    }
  }
  return -1;
}

/*-------------------------------------------------------------------------------------------------*/

int exec_chown(cmd_t cmd){
  int  indice_inode,i;
  uint right;
  if(cmd.nbArgs != 3){
    printf("erreur nombre parametres chown\n");
    return -1;
  }
  else{
    /*verifier les droits d'ecriture pour pouvoir editer*/
    indice_inode=search_inode(cmd.tabArgs[1]);
    if(indice_inode != -1){
      right=get_user_right(indice_inode);
      if(right == rW || right== RW){
        /*changer le propriétaire du fichier*/
        i=0;
        while(i<NB_USERS){
          if(strcmp(virtual_disk_sos.users_table[i].login,cmd.tabArgs[2]) == 0){
            virtual_disk_sos.inodes[indice_inode].uid=i;
            return 0;
          }
          i++;
        }
      }
    }
  }
  printf("utilisateur introuvable ou erreur droit d'acces\n");
  return -1;
}
/*-------------------------------------------------------------------------------------------------*/

int exec_chmod(cmd_t cmd){
  int  indice_inode;
  uint right ,r;
  if(cmd.nbArgs != 3){
    printf("erreur nombre parametres chmod\n");
    return -1;
  }
  else{
    /*verifier les droits d'ecriture pour pouvoir editer*/
    indice_inode=search_inode(cmd.tabArgs[1]);
    if(indice_inode != -1){
      right=get_user_right(indice_inode);
      if(right == rW || right== RW){
        /*changer les droits pour les autres*/
        if((r=getBinaryRight(cmd.tabArgs[2]))!= 2){
          virtual_disk_sos.inodes[indice_inode].oright=r;
          return 0;
        }
        else{
          printf("erreur parametre <droit>\n");
          return -1;
        }
      }
    }
  }
  printf("inode introuvable / erreur droit d'acces\n");
  return -1;
}
/*-------------------------------------------------------------------------------------------------*/

int exec_listusers(cmd_t cmd){
  if(cmd.nbArgs != 1){
    printf("erreur nombre parametres listusers\n");
    return -1;
  }
  for(int i=0; i<virtual_disk_sos.super_block.number_of_users;i++){
    printf("%s\n",virtual_disk_sos.users_table[i].login);
  }
  return 0;
}


/*-------------------------------------------------------------------------------------------------*/

int exec_adduser(cmd_t cmd){
  user_t user;
  char *passwd = malloc(30*sizeof(char));
  if(cmd.nbArgs != 1){
    printf("erreur nombre parametres adduser\n");
    return -1;
  }
  /*recuperer le login et le mot de passe*/
  printf("saisissez un Login :");
  fgets(user.login, FILENAME_MAX_SIZE, stdin);
  int j;
  for(j=0; user.login[j-1]!='\n'; j++){}
  user.login[j-1]='\0';

  printf("Password: ");
  fgets(passwd, 32*sizeof(char), stdin);
  int i;
  for(i=0; passwd[i-1]!='\n'; i++){}
  passwd[i-1]='\0';

  /*calculer le haché du mot de passe*/
  sha256ofString((BYTE *)passwd, user.passwd);
  if(add_user(user) == -1){
    printf("erreur ajout utilisateur\n");
    return -1;
  }
  printf("utilisateur ajouté :)\n");
  return 0;
}
/*-------------------------------------------------------------------------------------------------*/

int exec_rmuser(cmd_t cmd){
  if(cmd.nbArgs != 2){
    printf("erreur nombre parametres rmuser\n");
    return -1;
  }
  if(remove_user(cmd.tabArgs[1]) == -1){
    printf("erreur supression user\n");
    return -1;
  }
  return 0;
}
