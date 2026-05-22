/*
** EPITECH PROJECT, 2026
** POULER OR NOT
** File description:
** Ray
*/

#pragma once

class Ray {
    public:
        Ray();
        ~Ray();
    private:
        float ray_dir_x;
        float ray_dir_y;
        int map_x;
        int map_y;
        float side_dist_x;
        float side_dist_y;
        float delta_dist_x;
        float delta_dist_y;
        float perp_wall_dist;
        int step_x;
        int step_y;
        int side;
        int hit;
};
