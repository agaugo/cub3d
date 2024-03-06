# include "../../inc/cub3D.h"

void cub_exit(t_mlx *mlx)   // exit the game
{
	int i = 0;
	while (mlx->map->field[i])
		free(mlx->map->field[i++]); // free the map line by line
	free(mlx->map->field); // free the map
	free(mlx->map); // free the data structure
	free(mlx->player); // free the player structure
	free(mlx->ray); // free the ray structure
	mlx_delete_image(mlx->mlx_p, mlx->img); // delete the image
	mlx_close_window(mlx->mlx_p); // close the window
	mlx_terminate(mlx->mlx_p); // terminate the mlx pointer
	printf("Exited cub3D cleanly.\n"); // print the message
	exit(0);
}
