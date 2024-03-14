/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/07 17:30:23 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/14 20:23:15 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3D.h"
#include <float.h> 
#define RAY_LENGTH 100000

void	cub_set_pixel(t_mlx *mlx, int x, int y, int color)
{
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
		mlx_put_pixel(mlx->img, x, y, color);
}

int	rgb_to_hex2(int r, int g, int b)
{
	return (r << 24 | g << 16 | b << 8 | 255 << 0);
}

int xpm_char_to_color(char c) {
    switch (c) {
        case ' ': return rgb_to_hex2(40, 32, 12); // ' ' -> #28200C
        case '.': return rgb_to_hex2(56, 44, 20); // '.' -> #382C14
        case 'X': return rgb_to_hex2(64, 48, 24); // 'X' -> #403018
        case 'o': return rgb_to_hex2(72, 56, 24); // 'o' -> #483818
        case 'O': return rgb_to_hex2(84, 60, 28); // 'O' -> #543C1C
        case '+': return rgb_to_hex2(92, 64, 32); // '+' -> #5C4020
        case '@': return rgb_to_hex2(108, 72, 36); // '@' -> #6C4824
        case '#': return rgb_to_hex2(116, 76, 40); // '#' -> #744C28
        case '$': return rgb_to_hex2(144, 92, 52); // '$' -> #905C34
        default:  return rgb_to_hex2(255, 255, 255); // Fallback color, should not be reached
    }
}

int get_texture_x_coordinate(float ray_x, float ray_y, int textureWidth, float ray_angle, t_mlx *mlx, bool hitVertical) {
    float hit_point;
    if (hitVertical) {
        // Vertical wall hit
        hit_point = ray_y - floorf(ray_y / mlx->map->tile_size) * mlx->map->tile_size;
    } else {
        // Horizontal wall hit
        hit_point = ray_x - floorf(ray_x / mlx->map->tile_size) * mlx->map->tile_size;
    }

    int texX = (int)(hit_point * textureWidth / mlx->map->tile_size);
    if (texX >= textureWidth) texX = textureWidth - 1;

    // Correct for walls hit on opposite side
    if ((hitVertical && ray_angle > M_PI) || (!hitVertical && ray_angle > M_PI_2 && ray_angle < M_PI_2 * 3)) {
        texX = textureWidth - texX - 1;
    }

    return texX;
}


static char *xpmData[] = {
"64 64 9 1 ",
"  c #28200C",
". c #382C14",
"X c #403018",
"o c #483818",
"O c #543C1C",
"+ c #5C4020",
"@ c #6C4824",
"# c #744C28",
"$ c #905C34",
/* pixels */
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
"OOOOOOO.O .OOOOOOOOOOOOOO.O .OOOO.O .OOOOOO.O .OOOOOOOOOOO.O .OO",
"oo@####O$ O$+oooo##o@o@ooO$ O$#o#O$ O$+ooo+O$ O$o#oo#o#o##O$ O$@",
"oo@###+O$ O$@oooO##o@o@ooO$ O$#o#O$ O$#+o+#O$ O$o#oo#o#o##O$ O$@",
"oo+###OO$ O$#OooO##o@o@ooO$ O$@o#O$ O$##+##O$ O$o#oo#o#o##O$ O$@",
"OoO@##OO$ O$#Ooo+#@o@o@ooO$ O$@o#O$ O$#####O$ O$o#oo#o#o##O$ O$@",
"@oo+#+oO$ O$#+oO@#@o@o@ooO$ O$@o#O$ O$#####O$ O$o#oo#o#o@#O$ O$O",
"@oOo+OoO$ O$#@o+##+o@o@ooO$ O$+o#O$ O$#####O$ O$o#oo#o#o@#O$ O$O",
"#OooOooO$ O$##+@##+o@o@ooO$ O$+o#O$ O$#####O$ O$o#oo#o#o+#O$ O$O",
"#OooooOO$ O$##@###Oo@o@ooO$ O$oo#O$ O$#####O$ O$o#oo#o#o+#O$ O$o",
"@#oooO#O$ O$##@###Oo@o@ooO$ O$o+#O$ O$@####O$ O$o#oo#o#Oo#O$ O$o",
"@#OooO#O$ O$#####@oo@o@ooO$ O$o+#O$ O$@###@O$ O$o#oo#o#Oo#O$ O$o",
"+##oO##O$ O$#####@oo@o@ooO$ O$o##O$ O$+###@O$ O$o#oo#o#Oo#O$ O$o",
"O###O#+O$ O$@####@oo@o@ooO$ O$+##O$ O$o@#@+O$ O$o#oo#o#Oo#O$ O$O",
"o@##+#OO$ O$@####+oo@o@ooO$ O$+#@O$ O$o+@+oO$ O$o#oo#o#+o#O$ O$O",
"o@####OO$ O$+####+oo@o@ooO$ O$##@O$ O$oo+ooO$ O$o#oo#o#+o@O$ O$O",
"o+@###oO$ O$+####Ooo@o@ooO$ O$##+O$ O$oooooO$ O$o#oo#o#@o@O$ O$@",
"oO@##+oO$ O$O@###OoO@o@ooO$ O$#@+O$ O$oooooO$ O$o#oo#o#@o@O$ O$@",
"oo+##OoO$ O$O@##@ooO@o@ooO$ O$#@oO$ O$oooooO$ O$o#oo#o#@o+O$ O$@",
"ooO##OoO$ O$o+##@ooO@o@ooO$ O$#+oO$ O$+ooo+O$ O$o#oo#o##o+O$ O$@",
"OoO+OooO$ O$o+##+oo+@o@ooO$ O$@ooO$ O$@+o+@O$ O$o#oo#o@#ooO$ O$@",
"OooOooOO$ O$oO@@+oo+@o@ooO$ O$+ooO$ O$#@+@#O$ O$o#oo#o@#OoO$ O$@",
"OoooooOO$ O$oO++Ooo++o@ooO$ O$oooO$ O$##@##O$ O$o#oo#o+#OoO$ O$+",
"@ooooo@O$ O$ooOOooo@Oo@ooO$ O$oooO$ O$#####O$ O$o#oo#o+#+oO$ O$O",
"@ooooo@O$ O$ooooooo@Oo@ooO$ O$ooOO$ O$#####O$ O$o#oo#oO#+oO$ O$O",
"@OoooO+O$ O$ooooooO#oo@ooO$ O$ooOO$ O$#####O$ O$o#oo#oO#@OO$ O$o",
"+OoooOOO$ O$oooooo+#oo@ooO$ O$oo+O$ O$@####O$ O$o#oo#oO#@OO$ O$o",
"O@ooo@OO$ O$oooooo+#oo@ooO$ O$oo@O$ O$@####O$ O$o#oo#oo##+o$ O$o",
"O@Ooo@oO$ O$oooooo@#oo@ooO$ O$oO@O$ O$+###@O$ O$o#oo#oo##+O$ O$o",
"o+Oo@+oO$ O$Oooooo@#oo@ooO$ O$o+#O$ O$+@##@O$ O$o#oo#oo@#@O$ O$o",
"oO@O@OoO$ O$+ooooO#@oO@ooO$ O$O@#O$ O$o@##+O$ O$o#oo#oo@#@O$ O$o",
"oO@O@OoO$ O$@Oooo+#@oO@ooO$ O$+##O$ O$o+#@oO$ O$o#oo#Oo+##O$ O$o",
"oo+@+ooO$ O$#+ooO+#+o+@ooO$ O$@##O$ O$oo@+oO$ O$o#oo#Oo+##O$ O$O",
"ooO+OooO$ O$#@OoO##Oo+@ooO$ O$###O$ O$oo+ooO$ O$o#oo#OoO##O$ O$O",
"oooOoooO$ O$##+o+##Oo+@ooO$ O$##@O$ O$oooooO$ O$o#oo#OoO##O$ O$+",
"oooooooO$ O$###+###oo@@ooO$ O$##@O$ O$oooooO$ O$o#oo#+oO##O$ O$@",
"ooooooOO$ O$######@oo@@ooO$ O$##+O$ O$oooooO$ O$o#oo#+oo@#O$ O$@",
"OoooooOO$ O$@#####@oo#@ooO$ O$#@+O$ O$+ooooO$ O$o#oo#@oo@#O$ O$@",
"Oooooo+O$ O$@#####+oo#@ooO$ O$#@oO$ O$+ooooO$ O$o#oo@@oo+#O$ O$O",
"@OoooO+O$ O$+#####Ooo#+ooO$ O$#+oO$ O$@+oo+O$ O$o#oo@#oo+#O$ O$O",
"@OoooO@O$ O$O#####ooo#+ooO$ O$@+oO$ O$#+o++O$ O$o#oo+#OoO@O$ O$o",
"@+OoO+@O$ O$o@###@ooO#oooO$ O$@ooO$ O$#@++@O$ O$o#oo+#OoO@O$ O$o",
"@@OOO@+O$ O$o+###+oo+#oooO$ O$+ooO$ O$##+@#O$ O$o#ooO#OoO+O$ O$o",
"+@+O+@OO$ O$oO@#@Ooo@#oooO$ O$oooO$ O$##@##O$ O$o#ooO#+oo+O$ O$o",
"O@@+@@OO$ O$ooO@+ooo@#oooO$ O$ooOO$ O$#####O$ O$o#ooO#+OoOO$ O$o",
"O@@+@@oO$ O$oooOOooo#@oooO$ O$ooOO$ O$#####O$ O$o#ooO#@OoOO$ O$o",
"o@###@oO$ O$OooooooO#@oooO$ O$oo+O$ O$####@O$ O$o#ooo#@OoOO$ O$o",
"o+###@oO$ O$Ooooooo+#+oooO$ O$oO@O$ O$@###@O$ O$o#ooo@#+ooO$ O$o",
"oO@##@oO$ O$+oooooo@#+oooO$ O$o+#O$ O$@###+O$ O$o#ooo@#+OoO$ O$o",
"oO@##+oO$ O$@Oooooo@#ooooO$ O$O@#O$ O$+###+O$ O$o#ooo+#@OoO$ O$O",
"oo+#@+oO$ O$#OooooO#@ooooO$ O$@##O$ O$+##@OO$ O$o#ooo+#@+oO$ O$O",
"ooO@@OoO$ O$#+oooo+#@oo+oO$ O$###O$ O$O@#@OO$ O$o#OooO@#+OO$ O$+",
"ooO@+OoO$ O$@@Oooo@#+oo+oO$ O$###O$ O$O@#+oO$ O$o#OooO+#@OO$ O$+",
"ooo++ooO$ O$@#OooO@#ooo@oO$ O$###O$ O$o+@OoO$ O$o#Oooo+#@OO$ O$@",
"oooOOooO$ O$+#@OoO##ooo@oO$ O$###O$ O$oO+ooO$ O$o#OoooO@#OO$ O$@",
"OoooooOO$ O$o@#@O##@ooo@oO$ O$###O$ O$ooOooO$ O$o@+oooO+#@O$ O$#",
"OoooooOO$ O$o+#####+ooo#oO$ O$##@O$ O$oooooO$ O$o@@oooo+#@O$ O$#",
"+ooooo@O$ O$oo####@ooo+#oO$ O$##@O$ O$oooooO$ O$o+@OoooO@@O$ O$#",
"+OoooO@O$ O$oo+@#@+ooo@#oO$ O$#@+O$ O$oooooO$ O$o+#Ooooo++O$ O$#",
"@OoooO@O$ O$ooo+@+oooo@#oO$ O$#@+O$ O$oooooO$ O$oo#OoooooOO$ O$#",
"@+oooO@O$ O$oooo+oooo@##+O$ O$@+oO$ O$oooooO$ O$+o#+ooooooO$ O$#",
"+++++++O$ O$+ooooooo+++++O$ O$+ooO$ O$+ooo+O$ O$+o++ooooooO$ O$+"
};

void cub_draw_vertical_line(t_mlx *mlx, int x, float wall_height, int texX) {
    // Adjusted to include texX for correct texture mapping.
    int textureWidth = 64; // Assuming the texture is square
    int textureHeight = 64; // Assuming fixed height from your XPM data
    int y, texY;
    int start = (SCREEN_HEIGHT / 2) - (wall_height / 2);
    int end = start + wall_height;
    float step = (float)textureHeight / wall_height; // Scaling factor for texture
    float texPos = 0; // Start at the top of the texture

    if (start < 0)
        start = 0;
    if (end > SCREEN_HEIGHT)
        end = SCREEN_HEIGHT;

    // Draw ceiling
    for (y = 0; y < start; ++y)
        cub_set_pixel(mlx, x, y, mlx->map->color_ceiling);

    // Draw wall with texture
    for (y = start; y < end; ++y) {
        texY = (int)texPos & (textureHeight - 1);
        texPos += step;
        char pixelChar = xpmData[texY + 4][texX % textureWidth]; // Use modulus to wrap texX
        int color = xpm_char_to_color(pixelChar);
        cub_set_pixel(mlx, x, y, color);
    }

    // Draw floor
    for (y = end; y < SCREEN_HEIGHT; ++y)
        cub_set_pixel(mlx, x, y, mlx->map->color_floor);
}

void	cub_handle_wall_found(t_mlx *mlx, float ray_x, float ray_y, float ray_angle, int ray_num, float projection_distance, bool hitVertical)
{
	float delta_x;
	float delta_y;
	float direct_distance;
	float perpendicular_distance;
	float wall_height;
	
	delta_x = ray_x - mlx->player->pixel_x;
	delta_y = ray_y - mlx->player->pixel_y;
	direct_distance = sqrt(delta_x * delta_x + delta_y * delta_y);
	perpendicular_distance = direct_distance * cos(ray_angle - mlx->player->angle);
	wall_height = (mlx->map->tile_size / perpendicular_distance) * projection_distance;

    int texX = get_texture_x_coordinate(ray_x, ray_y, 64, ray_angle, mlx, hitVertical);
    cub_draw_vertical_line(mlx, ray_num, wall_height, texX);
}

void cub_cast_single_ray(t_mlx *mlx, float ray_angle, int ray_num, float projection_distance) {
    float ray_x = mlx->player->pixel_x;
    float ray_y = mlx->player->pixel_y;
    bool hitVertical = false; // A flag for the type of wall hit
    float wall_hit_x = 0;
    float wall_hit_y = 0;
    int map_x, map_y;
    int i = 0;

    // Calculate the step increment for ray casting
    float x_step = cos(ray_angle);
    float y_step = sin(ray_angle);

    while (i++ < RAY_LENGTH) {
        map_x = (int)ray_x / mlx->map->tile_size;
        map_y = (int)ray_y / mlx->map->tile_size;

        // Check if the ray has hit a wall
        if (mlx->map->field[map_y][map_x] == '1') {
            // Save the wall hit coordinates
            wall_hit_x = ray_x;
            wall_hit_y = ray_y;
            // Calculate the distances to the horizontal and vertical wall hits
            float horizontal_hit_distance = FLT_MAX;
            float vertical_hit_distance = FLT_MAX;

            // Calculate horizontal hit distance
            if (x_step != 0) {
                float dx = (x_step > 0) ? (map_x * mlx->map->tile_size + mlx->map->tile_size - mlx->player->pixel_x) : (mlx->player->pixel_x - map_x * mlx->map->tile_size);
                horizontal_hit_distance = dx / x_step;
            }

            // Calculate vertical hit distance
            if (y_step != 0) {
                float dy = (y_step > 0) ? (map_y * mlx->map->tile_size + mlx->map->tile_size - mlx->player->pixel_y) : (mlx->player->pixel_y - map_y * mlx->map->tile_size);
                vertical_hit_distance = dy / y_step;
            }

            // Determine the direction of the wall hit based on which distance is shorter
            hitVertical = (vertical_hit_distance < horizontal_hit_distance);

            // Call the function to handle the wall hit
            cub_handle_wall_found(mlx, wall_hit_x, wall_hit_y, ray_angle, ray_num, projection_distance, hitVertical);
            break;
        }

        // Increment the ray casting steps
        ray_x += x_step;
        ray_y += y_step;
    }
}


void	cub_init_ray_casting(t_mlx *mlx, float *ray_angle, float *angle_increment, float *projection_distance)
{
	*ray_angle = mlx->player->angle - (FOV / 2.0 * (M_PI / 180.0));
	*angle_increment = FOV / (float)SCREEN_WIDTH * (M_PI / 180.0);
	*projection_distance = (SCREEN_WIDTH / 2) / tan(FOV / 2.0 * (M_PI / 180.0));
}

void	cub_cast_rays(t_mlx *mlx)
{
	float ray_angle;
	float angle_increment;
	float projection_distance;
	int ray_num = 0;
	
	cub_init_ray_casting(mlx, &ray_angle, &angle_increment, &projection_distance);
	while (ray_num++ < SCREEN_WIDTH)
	{
		ray_angle = fmod(ray_angle, 2.0 * M_PI);
		if (ray_angle < 0)
			ray_angle += 2.0 * M_PI;
		cub_cast_single_ray(mlx, ray_angle, ray_num, projection_distance);
		ray_angle += angle_increment;
	}
}
