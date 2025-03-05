/**
 * @file couche3.c
 * @brief Couche permettant de gérer les utilisateurs à travers une table des utilisateurs.
 */

#ifndef __COUCHE3_H__
#define __COUCHE3_H__

/**
 * @brief fonction pour initiser la table des utilisateurs --> initialisation de root
 */
void init_users_table();

/**
 * @brief
 * @return int
 */
int read_user_table();

/**
 * @brief
 * @return int
 */
int write_user_table();

/**
 * @brief fonction qui ajoute user à la table users_table -->return -1 en cas de probleme sinon 0
 * @param user
 * @return int
 */
int add_user(user_t user);

/**
 * @brief fonction qui supprime le user de nom login -->return -1 en cas de probleme sinon 0
 * @param login
 * @return int
 */
int remove_user(char *login);

#endif
