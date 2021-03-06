/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgerace <sgerace@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 16:33:26 by sgerace           #+#    #+#             */
/*   Updated: 2022/07/01 20:02:31 by sgerace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*DA RICORDARE:
    per ragioni di norma la funzione empty_checker è diventata empty_or_invalid_checker svolgendo DUE funzioni
    se la mappa è vuota il counter delle righe viene messo manualmente = 1
    dove ci sono i = -1 servono per non andare in seg fault, di solito nella condizione del while ci sarà un i++ così controlla se è vera o falsa e la pone = 0
*/
#include "so_long.h"

int count_rows()
{
	int		counter;
    int     fd;
	char	*row;

    counter = 0;
	fd = open("./maps/map1.ber", O_RDONLY);
	row = get_next_line(fd, 1);
    if (row == NULL)    //questo serve perchè anche se la mappa è vuota per fare almeno un controllo counter deve essere almeno 1
    {
        counter++;
    }
	while (row)
	{
		row = get_next_line(fd, 1);
		counter++;
	}
	close(fd);
	free(row);
    return (counter);
}

int items_counter(char c)
{
    static int p_toggler = 0;   //lo trova e aumenta di 1, alla fine se non è 1 o non lo ha trovato mai o l ha trovato troppe volte
    static int e_toggler = 0;
    static int c_toggler = 0;

    if (c == 'P')
        p_toggler++;
    else if (c == 'E')
    {
        e_toggler++;
        return (1); //se è presente almeno una volta verrà ritornato il valore e potrò controllare la variabile ad esso associata, se è 1 vuol dire che c'è almeno un'uscita
    }
    else if (c == 'C')
    {
        c_toggler++;
        return (2); //se è presente almeno una volta verrà ritornato il valore e potrò controllare la variabile ad esso associata, se è 2 vuol dire che c'è almeno un collezionabile
    }
    return (p_toggler);
}

int items_checker(s_map  *map_ptr)
{
    int i;
    int j;

    i = -1;
    map_ptr->rows = count_rows();
    while(i < map_ptr->rows - 1 && map_ptr->map[i++])
    {
        j = -1;
        while(map_ptr->map[i][++j] != '\n')
        {
            if (map_ptr->map[i][j] == 'P')
                map_ptr->p = items_counter(map_ptr->map[i][j]);
            else if (map_ptr->map[i][j] == 'E')
                map_ptr->e = items_counter(map_ptr->map[i][j]);
            else if (map_ptr->map[i][j] == 'C')
                map_ptr->c = items_counter(map_ptr->map[i][j]);
        }
    }
    if (map_ptr->p != 1 || map_ptr->e != 1 || map_ptr->c != 2)
        return (1);
    return (0);
}

int empty_or_invalid_checker(s_map  *map_ptr)
{
    int i;
    int j;

    if (map_ptr->map[0] == NULL)
        return (1);
    i = -1;
    map_ptr->rows = count_rows();
    while(i < map_ptr->rows - 1 && map_ptr->map[i++])
    {
        j = -1;
        while(map_ptr->map[i][++j] != '\n')
        {
            if (!(map_ptr->map[i][j] == '0' || map_ptr->map[i][j] == '1' || \
                map_ptr->map[i][j] == 'P' || map_ptr->map[i][j] == 'E' \
                || map_ptr->map[i][j] == 'C'))
                return (2);
        }
    }
    return (0);
}

int walls_checker(s_map *map_ptr)
{
    int i = 0;

    map_ptr->rows = count_rows() - 1; //tolgo uno perchè gli array partono da 0 e questo lo uso come indice
    if(ft_findchar(map_ptr->map[0], '1') == 1)
        return (1);
    if(ft_findchar(map_ptr->map[map_ptr->rows], '1') == 1)
        return (3);
    while (i < map_ptr->rows && map_ptr->map[i++])
    {
        if (((map_ptr->map[i][0]) != '1') || ((map_ptr->map[i][ft_strlen(map_ptr->map[i]) - 2]) != '1'))
            return (2);
    }
    return (0);
}

int rectangle_checker(s_map  *map_ptr)
{
    int i = 0;
    size_t tmp = 0;

    map_ptr->rows = count_rows();
    tmp = ft_strlen(map_ptr->map[0]);
    while (map_ptr->map[i++])
	{
        if (tmp != ft_strlen(map_ptr->map[i]) && \
         i < map_ptr->rows && map_ptr->map[i][0] != '\n')
            return (1);
	}
    return (0);
}

int check_map_errors(s_map  *map_ptr)
{
    if (empty_or_invalid_checker(map_ptr) != 0)
        return (1);
    if (rectangle_checker(map_ptr) != 0)
        return (1);
    if (walls_checker(map_ptr) != 0)
        return (1);
    if (items_checker(map_ptr) != 0)
        return (1);
    return (0);
}

char **upload_map(s_map  *map_ptr)
{
	int		    fd;
	int		    i;

    i = -1; //i = -1 così posso risparmiare un rigo e aumentare la i nella condizione del while 4 righe in basso
    map_ptr->rows = count_rows();
	fd = open("./maps/map1.ber", O_RDONLY);
	map_ptr->map = (char **) malloc (sizeof(char *) * (map_ptr->rows + 1));
	while (i++ < map_ptr->rows - 1)
        map_ptr->map[i] = get_next_line(fd, 1);
    close(fd);
	return (map_ptr->map);
}

int main(void)
{
    s_map *map_ptr;

    map_ptr = (s_map *) malloc (sizeof(s_map));
    map_ptr->map = upload_map(map_ptr);
    if(check_map_errors(map_ptr) == 1)
        ft_printf("Error\n");
    else
        ft_printf("Valid\n");
}

//->