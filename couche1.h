/**
 * @file couche1.c
 * @brief Couche qui permet de gérer les blocs sur le système.
 */


#ifndef __COUCHE1_H__
#define __COUCHE1_H__
#define TOUT_VA_BIEN 0
#define ERREUR_ECRITURE 2
#define ERREUR_LECTURE 3

/**
 * @brief Fonction pour initialiser le virtuel disk d0 a partir du repertoire dirname
 * @param dirname Chaine de caractere qui contient le nom du repertoire
 * @return virtual_disk_t virtual_disk_sos
 */
void init_disk_sos(char *dirname);

/**
 * @fn init_disk_sos(char *dirname)
 * @brief fonction qui écrit un bloc block, à la position pos sur le disque du système
 * @param block = Block qui sera écrit
 * @param pos = Position où on écrira le fichier
 * @return int = Nombre de blocs écris , ou -1 en cas de problèmes
 */
int write_block(block_t block, long pos);

/**
 * @brief fonction qui lit un bloc block de données, à la position pos sur le disque du système
 * @param block = Endroit où est enregistré le Block lu
 * @param pos = Position dans le fichier où le block est lu
 * @return int
 */
int read_block(block_t *block,long pos);

/**
 * @brief fonction pour calculer le nombre de blocks ncessaires à partir d'in nombre d'octects
 * @param nb_octects = Nombre d'octets pour lequel on rechercher le nombre de blocs
 * @return uint = renvoie le nombre de blocs pour le nombre d'octets donné en entrée
 */
uint compute_nblock(uint nb_octects);

/**
 * @brief fonctions d’affichage a l’ecran des blocs de type block t en hexadecimal
 * @param pos =Permet d'afficher un bloc à une position donnée
 */
void afficher_block(long pos);

void verify_upload();

#endif
