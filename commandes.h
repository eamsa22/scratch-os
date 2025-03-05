/**
 * @file couche5.c
 * @brief Couche permettant de réaliser l'implémentation du terminal et des différentes
 */

#ifndef __COUCHE5_H__
#define __COUCHE5_H__

#include "sos_defines.h"

char *getRight(uint right);

uint getBinaryRight(char *right);

uint get_user_right(int indice_inode);


/**
fonctions dédiées à chaque commande
*/
int exec_ls(cmd_t cmd);
int exec_cat(cmd_t cmd);
int exec_chown(cmd_t cmd);
int exec_chmod(cmd_t cmd);
int exec_listusers(cmd_t cmd);
int exec_quit(cmd_t cmd);
int exec_adduser(cmd_t cmd);
int exec_rmuser(cmd_t cmd);

#endif
