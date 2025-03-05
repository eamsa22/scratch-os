/**
 * @file couche4.c
 * @brief Couche comprenant les fonctions qui gèrent les différents fichiers (écriture, lecture, suppression)
 */

#ifndef __COUCHE4_H__
#define __COUCHE4_H__
#define ERREUR_ECRITURE 2
#define ERREUR_LECTURE 3

/**
 * @brief fonction renvoie l'indice de l'inode correspond à File_name --> return -1 cas introuvable
 * @param File_name
 * @return int
 */
int search_inode(char *File_name);

/**
 * @brief fonction qui écrit un fichier file sur le disque --> return 0 en cas de probleme 1 sinon
 * @param File_name
 * @param file
 * @return int
 */
int write_file(char *File_name ,file_t file);

/**
 * @brief fonction qui lit un fichier nommé File_name qui est sur le disque --> return 0 en cas de probleme 1 sinon
 * @param File_name
 * @param file
 * @return int
 */
int read_file(char *File_name , file_t *file);

/**
 * @brief fonction qui supprime un fichier nommé File_name qui est sur le disqu--> return 0 en cas de probleme 1 sinon
 * @param File_name
 * @return int
 */
int delete_file(char *File_name);

/**
 * @brief fonction qui écrit un fichier qui est sur l'ordinateur host sur le disque --> return 0 en cas de probleme 1 sinon
 * @param File_name
 * @return int
 */
int load_file_from_host(char *File_name);

/**
 * @brief fonction qui écrit un fichier qui est sur le disque sur l'ordinateur host  --> return 0 en cas de probleme 1 sinon
 * @param File_name
 * @return int
 */
int store_file_to_host(char *File_name);

/**
 *@brief
 *
 *return int
 */
int defragmentateur();

#endif
