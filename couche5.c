#include "sos_defines.h"
#include "couche4.h"
#include "couche2.h"
#include "couche1.h"
#include "timestamp.h"
#include "commandes.h"

session_t session;

int executer_cmd(cmd_t cmd){
  if(cmd.nbArgs >= 1){
    if(strcmp(cmd.tabArgs[0],"ls")== 0){
      return exec_ls(cmd);
    }
    else if(strcmp(cmd.tabArgs[0],"cat")== 0){
      return exec_cat(cmd);
    }
    else if(strcmp(cmd.tabArgs[0],"chown")== 0){
      return exec_chown(cmd);
    }
    else if(strcmp(cmd.tabArgs[0],"chmod")== 0){
      return exec_chmod(cmd);
    }
    else if(strcmp(cmd.tabArgs[0],"listusers")== 0){
      return exec_listusers(cmd);
    }
    else if(strcmp(cmd.tabArgs[0],"adduser")== 0){
      return exec_adduser(cmd);
    }
    else if(strcmp(cmd.tabArgs[0],"rmuser")== 0){
      return exec_rmuser(cmd);
    }
    else{
      return -1;
    }
  }
  else{
    return -1;
  }
}
/*-------------------------------------------------------------------------------------------------*/

int Decoupe(char Chaine[], char *pMots[]){
  char *p;
  int NbMots = 0;
  p = Chaine; /* On commence par le debut */
  /* Tant que la fin de la chaine n'est pas atteinte et qu'on ne deborde pas */
  while ((*p) != '\0' && (*p) != '\n' && NbMots < 80){
    while ((*p) == ' ' && (*p) != '\n' && (*p) != '\0') p++; /* Recherche du debut du mot */
    if ((*p) == '\0') break; /* Fin de chaine atteinte */
    pMots[NbMots++] = p; /* Rangement de l'adresse du 1er caractere du mot */
    while ((*p) != ' ' && (*p) != '\n' && (*p) != '\0') p++; /* Recherche de la fin du mot */
    if ((*p) == '\0') break; /* Fin de chaine atteinte */
    *p = '\0'; /* Marquage de la fin du mot */
    p++; /* Passage au caractere suivant */
  }
  pMots[NbMots] = NULL; /* Derniere adresse */
  return NbMots;
}

/*-------------------------------------------------------------------------------------------------*/

/**
fonction qui verifie si le login et password correctes ->renvoie -1 si non trouvé sinon 0
et stocke dans session le id du user
*/
int verifier_user(){
  user_t user;
  char *passwd = malloc(30*sizeof(char));

  /*demander un login et un mot de passe à l'utilisateur*/
  printf("Login: ");
  fgets(user.login, FILENAME_MAX_SIZE, stdin);

  int j;
  for(j=0; user.login[j-1]!='\n'; j++){
  }
  user.login[j-1]='\0';

  printf("Password: ");
  fgets(passwd, 30*sizeof(char), stdin);

  int i;
  for(i=0; passwd[i-1]!='\n'; i++){
  }
  passwd[i-1]='\0';

  /*calculer le haché du mot de passe*/
  sha256ofString((BYTE *)passwd, user.passwd);

  /*verifier si le login existe et verifier le haché*/
  for(int i=0; i<virtual_disk_sos.super_block.number_of_users; i++){
    if(strcmp(virtual_disk_sos.users_table[i].login, user.login) == 0){
      if(strcmp(virtual_disk_sos.users_table[i].passwd, user.passwd) == 0){
        session.userid = i;
        return 0;
      }
    }
  }
  return -1;
}
/*-------------------------------------------------------------------------------------------------*/

int verif_demarer_interprete_cmd(){
  int tentatives=3;
  int verif;

  while(tentatives > 0 && ((verif=verifier_user()) == -1) ){
    printf("Login ou Password incorect, saisissez à nouveau !\n");
    tentatives-=1;
  }
  return verif;
}
/*-------------------------------------------------------------------------------------------------*/
int lancer_interprete_cmd(){
  cmd_t commande;
  char cmd_with_param[80];

  int resultat;
  int est_demarer = verif_demarer_interprete_cmd();
  if(est_demarer == 0){

    /*lancer l'interpreteur de commande*/
    printf("%s@ScratchOS:~$ ", virtual_disk_sos.users_table[session.userid].login);
    fgets(cmd_with_param,80,stdin);
    /*char_cmd_with_param == "cat file.txt"*/
    /*construire la structure permettant de gerer la commande*/
    /*ex: commande.tabArgs[]==[[c,a,t],[f,i,l,e,.,t,x,t]]*/
    /*       commande.nbArgs==2*/
    commande.nbArgs = Decoupe(cmd_with_param, commande.tabArgs);

    while(strcmp(commande.tabArgs[0], "quit") != 0){
      resultat = executer_cmd(commande);
      if(resultat == -1){
        printf("%s: Command not found or can not be executed:(\n",commande.tabArgs[0]);
      }
      printf("%s@ScratchOS:~$ ", virtual_disk_sos.users_table[session.userid].login);
      fgets(cmd_with_param,80,stdin);

      commande.nbArgs = Decoupe(cmd_with_param, commande.tabArgs);
    }
    /*executer quit*/
  }
  return est_demarer;
}
/*-------------------------------------------------------------------------------------------------*/
