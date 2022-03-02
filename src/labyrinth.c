//
// Created by alice.B on 22/02/2022.
//
#include <time.h>

#include "labyrinth.h"

labyrinth *labyrinth_create(uint32 width, uint32 height, BOOL two_player) {

    if (width < 1 || height < 1) {
        fprintf(stderr, "width and height must be positive and not null");
        return NULL;
    }

    labyrinth *self = malloc(sizeof(struct labyrinth));
    srand(time(NULL) + 45872);

    self->width = width;
    self->height = height;
    self->start_case = 0;
    if (two_player) {
        self->start_case2 = (width * height) - width;
        self->finish_case = (width * (height / 2)) - 1;
    }
    else {
        self->start_case2 = 0;
        self->finish_case = (width * height) - 1;
    }

    self->is_solve = FALSE;
    self->max_distance = 0;
    self->generator_tmp = NULL;

    // alloc table for al case
    self->cases = malloc(width * height * sizeof(uint32));

    // set all
    labyrinth_reset(self);
    return self;
}


labyrinth *labyrinth_create_from_file(const char *filename, int two_player) {
    FILE * inputFile;
    inputFile = fopen(filename, "rb");
    if (inputFile == NULL) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        return NULL;
    }
    char * body;

    uint32 header[5];

    if (fread(header, sizeof(uint32), 5, inputFile) != 5) goto err_read;

    labyrinth *self = malloc(sizeof(struct labyrinth));

    self->width = header[0];
    self->height = header[1];
    self->start_case = header[2];
    self->start_case2 = header[3];
    self->finish_case = header[4];

    self->is_solve = FALSE;
    self->max_distance = 0;
    self->generator_tmp = NULL;

    size_t size = self->width * self->height;

    body = malloc(size);
    if (fread(body, 1, size, inputFile) != size) goto err_read;

    self->cases = malloc(size * sizeof(uint32));

    for (size_t cpt = 0 ; cpt < size ; ++cpt) {
        self->cases[cpt] = (uint32) body[cpt];
    }

    goto end;

err_read:
    fprintf(stderr, "Error when reading file %s\n", filename);
    free(self);
    self = NULL;

end:
    fclose(inputFile);
    inputFile = NULL;
    if (body != NULL) {
        free(body);
        body = NULL;
    }

    return self;
}


/**
 * @deprecated stop use this function
 * @param tables array
 * @param size size of the array
 * @return sum of the array values
 */
//uint32 sum_tables(const uint32 *tables, uint32 size) {
//    uint32 sum = 0;
//    // make teh sum of all the value
//    for (int cpt = 0; cpt < size; ++cpt) sum += tables[cpt];
//    return sum;
//}

BOOL is_generated(const uint32 *tables, uint32 size) {
    for (int cpt = 0; cpt < size; ++cpt) if (tables[cpt] != 0) return FALSE;
    return TRUE;
}


int labyrinth_get_directional_index(const labyrinth *self, uint32 current, direction direction) {
    // get x and y from index
    uint32 x = current % self->width;
    uint32 y = current / self->width;

    switch (direction) {
        // for each check if steel in bound else return -1
        case DIRECTION_TOP:
            if (y < 1) return -1;
            y -= 1;
            break;
        case DIRECTION_DOWN:
            if (y >= self->height - 1) return -1;
            y += 1;
            break;
        case DIRECTION_LEFT:
            if (x < 1) return -1;
            x -= 1;
            break;
        case DIRECTION_RIGHT:
            if (x >= self->width - 1) return -1;
            x += 1;
            break;
        default:
            return -1;
    }
    // get index from x and y
    return (int) (x + y * self->width);
}


void replace_value_tables(uint32 *tab, uint32 size, uint32 old_value, uint32 new_value) {
    for (int cpt = 0; cpt < size; ++cpt) if (tab[cpt] == old_value) tab[cpt] = new_value;
}

short get_opposite_direction(direction direction) {
    if (direction == DIRECTION_TOP) return DIRECTION_DOWN;
    if (direction == DIRECTION_DOWN) return DIRECTION_TOP;
    if (direction == DIRECTION_RIGHT) return DIRECTION_LEFT;
    if (direction == DIRECTION_LEFT) return DIRECTION_RIGHT;
    // invalid direction return -1
    return -1;
}

BOOL labyrinth_generator_Kruskal(labyrinth *self, int step_by_step) {
    if (self == NULL) {
        fprintf(stderr, "null pointer");
        return TRUE;
    }

    uint32 size = self->width * self->height;

    // create table with same size of the labyrinth
    if (self->generator_tmp == NULL) {
        self->generator_tmp = malloc(size * sizeof(uint32));
        // set different positive number in each case with one et at 0
        for (int cpt = 0; cpt < size; ++cpt) {
            self->generator_tmp[cpt] = cpt;
        }
    }

    int step_count = 0;

    // if th sum equal to 0 mean that all case are reachable from any case
    while (!is_generated(self->generator_tmp, size)) {
        // get random case and random direction
        uint32 current_case = rand() % size;
        direction border = 1 << (random() % 4);
        // if the border is already remove ignore
        if (self->cases[current_case] & border) {
            // get the case in the select direction
            int new_case = labyrinth_get_directional_index(self, current_case, border);
            // if case not exist or current case value isn't same as new case value
            if (new_case != -1 && self->generator_tmp[new_case] != self->generator_tmp[current_case]) {
                // get the min and max value between current case and new case
                uint32 min_value;
                uint32 max_values;
                if (self->generator_tmp[new_case] < self->generator_tmp[current_case]) {
                    min_value = self->generator_tmp[new_case];
                    max_values = self->generator_tmp[current_case];
                } else {
                    min_value = self->generator_tmp[current_case];
                    max_values = self->generator_tmp[new_case];
                }
                // remove borders
                self->cases[current_case] &= ~border;
                self->cases[new_case] &= ~get_opposite_direction(border);
                // replace the max value by the min
                replace_value_tables(self->generator_tmp, size, max_values, min_value);
                step_count++;
                if (step_by_step > 0 && step_count >= step_by_step) goto end;
            }
        }
    }
    // free the tables
    end:
    if (is_generated(self->generator_tmp, size)) {
        if (self->generator_tmp != NULL) {
            free(self->generator_tmp);
            self->generator_tmp = NULL;
        }
        return TRUE;
    }
    return FALSE;
}


BOOL labyrinth_can_pass_through(const labyrinth *self, uint32 pos, direction direction) {
    // valid pos
    if (pos < 0 || pos >= self->width * self->height) return FALSE;
    // not a border and no wall in the direction
    return (labyrinth_get_directional_index(self, pos, direction) != -1) && !(self->cases[pos] & direction);
}

void labyrinth_render(SDL_Renderer *renderer, const labyrinth *lab, const player* player1, const player* player2, const rec *render_rec) {

    // max size for width
    int sx = (render_rec->x2 - render_rec->x1) / (int) lab->width;
    // max size for height
    int sy = (render_rec->y2 - render_rec->y1) / (int) lab->height;
    // get min size between width and height
    int case_size = sx > sy ? sy : sx;

    //render start case
    SDL_SetRenderDrawColor(renderer, 0, 200, 0, SDL_ALPHA_OPAQUE);
    SDL_Rect rect = {(int) (lab->start_case % lab->width) * case_size + render_rec->x1,
                     (int) (lab->start_case / lab->width) * case_size + render_rec->y1,
                     case_size, case_size};
    SDL_RenderFillRect(renderer, &rect);

    if (lab->start_case != lab->start_case2)
    {
        rect = (SDL_Rect) {(int) (lab->start_case2 % lab->width) * case_size + render_rec->x1,
                         (int) (lab->start_case2 / lab->width) * case_size + render_rec->y1,
                         case_size, case_size};
        SDL_RenderFillRect(renderer, &rect);
    }

    // render end case
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, SDL_ALPHA_OPAQUE);
    rect = (SDL_Rect) {(int) (lab->finish_case % lab->width) * case_size + render_rec->x1,
                       (int) (lab->finish_case / lab->width) * case_size + render_rec->y1,
                       case_size, case_size};
    SDL_RenderFillRect(renderer, &rect);

    // render players
    if (player1 != NULL && player2 != NULL && *player1 == *player2) {
        // if both player is on same case
        SDL_SetRenderDrawColor(renderer, 169, 62, 184, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &(SDL_Rect) {(int) (*player1 % lab->width) * case_size + case_size / 4 + render_rec->x1,
                                                 (int) (*player1 / lab->width) * case_size + case_size / 4 + render_rec->y1,
                                                 case_size / 4, case_size / 2});
        SDL_SetRenderDrawColor(renderer, 161, 171, 48, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &(SDL_Rect) {(int) (*player2 % lab->width) * case_size + case_size / 2 + render_rec->x1,
                                                  (int) (*player2 / lab->width) * case_size + case_size / 4 + render_rec->y1,
                                                  case_size / 4, case_size / 2});
    } else {

        if (player1 != NULL)
        {
            SDL_SetRenderDrawColor(renderer, 169, 62, 184, SDL_ALPHA_OPAQUE);
            rect = (SDL_Rect) {(int) (*player1 % lab->width) * case_size + case_size / 4 + render_rec->x1,
                               (int) (*player1 / lab->width) * case_size + case_size / 4 + render_rec->y1,
                               case_size / 2, case_size / 2};
            SDL_RenderFillRect(renderer, &rect);
        }

        if (player2 != NULL) {
            SDL_SetRenderDrawColor(renderer, 161, 171, 48, SDL_ALPHA_OPAQUE);
            rect = (SDL_Rect) {(int) (*player2 % lab->width) * case_size + case_size / 4 + render_rec->x1,
                               (int) (*player2 / lab->width) * case_size + case_size / 4 + render_rec->y1,
                               case_size / 2, case_size / 2};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // render border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    for (int x = 0; x < lab->width; x++) {
        for (int y = 0; y < lab->height; y++) {

            // get pos of each vertex
            int cx = x * case_size, ocx = cx + case_size;
            int cy = y * case_size, ocy = cy + case_size;

            uint32 index = x + y * lab->width;

            if (lab->is_solve) {
                if (lab->cases[index] & IS_IN_PATH_MASK) {
                    SDL_SetRenderDrawColor(renderer, 194, 21, 21, SDL_ALPHA_OPAQUE);
                    SDL_RenderFillRect(renderer, &(SDL_Rect) {cx, cy, case_size, case_size});
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                } else {
                    uint8 color = (uint8) (((double) labyrinth_get_distance(lab, index) / lab->max_distance) * 255.0);
                    if (color != 0 && index != lab->start_case && index != lab->finish_case) {
                        SDL_SetRenderDrawColor(renderer, 45, color, 185, SDL_ALPHA_OPAQUE);
                        SDL_RenderFillRect(renderer, &(SDL_Rect) {cx, cy, case_size, case_size});
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                    }
                }

            }

            uint16 border = lab->cases[index];
            // render border
            if (border & DIRECTION_LEFT) SDL_RenderDrawLine(renderer, cx, cy, cx, ocy);
            if (border & DIRECTION_RIGHT) SDL_RenderDrawLine(renderer, ocx, cy, ocx, ocy);
            if (border & DIRECTION_TOP) SDL_RenderDrawLine(renderer, cx, cy, ocx, cy);
            if (border & DIRECTION_DOWN) SDL_RenderDrawLine(renderer, cx, ocy, ocx, ocy);
        }
    }
}

void labyrinth_reset(labyrinth *self) {
    // set all wall in each case
    for (int cpt = 0; cpt < self->width * self->height; ++cpt) self->cases[cpt] = DIRECTION_ALL;
    if (self->generator_tmp != NULL) {
        free(self->generator_tmp);
        self->generator_tmp = NULL;
    }
    self->is_solve = FALSE;
    self->max_distance = 0;
}

uint16 labyrinth_get_distance(const labyrinth *self, uint32 index) {
    return (self->cases[index] & DISTANCE_MASK) >> DISTANCE_OFFSET;
}

void labyrinth_set_distance(labyrinth *self, uint32 index, uint16 distance) {
    // clear current distance
    self->cases[index] &= ~DISTANCE_MASK;
    // set new distance
    self->cases[index] |= (((uint32) distance) << DISTANCE_OFFSET) & DISTANCE_MASK;
}

BOOL is_labyrinth_solve_end(labyrinth *self, int end_index) {
    if (end_index >= 0) return (BOOL) (self->cases[end_index] & DISTANCE_MASK);
    for (int cpt = 0; cpt < self->width * self->height; ++cpt)
        if (!(self->cases[cpt] & DISTANCE_MASK)) return FALSE;
    return TRUE;
}

void labyrinth_calc_max_distance(labyrinth *self) {
    self->max_distance = 0;
    uint32 distance;
    if (self->is_solve) {
        for (int cpt = 0; cpt < self->width * self->height; ++cpt) {
            distance = labyrinth_get_distance(self, cpt);
            if (distance > self->max_distance) self->max_distance = distance;
        }
    }
}

/**
 * generate distance map values
 * @param self labyrinth instance pointer
 * @param end_index stop algo when this case is reach negative number for all case
 * @parm step_by_step make one distance every time function it call
 */
BOOL labyrinth_solve(labyrinth *self, int end_index, BOOL step_by_step) {

    self->is_solve = TRUE;

    // clear old value
    if (!step_by_step)
        for (int cpt = 0; cpt < self->width * self->height; ++cpt) self->cases[cpt] &= ~DISTANCE_MASK;

    uint16 distance = self->max_distance;

    // set distance for first case if is teh first iteration for the step by step
    if (distance == 0) {
        distance = 1;
        labyrinth_set_distance(self, self->finish_case, distance);
    }

    int edit_count;
    direction direction;

    while (!is_labyrinth_solve_end(self, end_index)) {
        distance++;
        edit_count = 0;
        for (int cpt = 0; cpt < self->width * self->height; ++cpt) {
            if (!(self->cases[cpt] & DISTANCE_MASK)) {
                for (int m = 0; m < 4; m++) {
                    direction = 1 << m;
                    if (labyrinth_can_pass_through(self, cpt, direction) &&
                        labyrinth_get_distance(self, labyrinth_get_directional_index(self, cpt, direction)) ==
                        distance - 1) {
                        edit_count++;
                        labyrinth_set_distance(self, cpt, distance);
                    }
                }
            }
        }
        if (!edit_count || step_by_step) goto end;
    }
    end:
    labyrinth_calc_max_distance(self);
    return is_labyrinth_solve_end(self, end_index);
}

void labyrinth_destroy(labyrinth *self) {
    // free
    if (self == NULL) return;

    if (self->generator_tmp != NULL) {
        free(self->generator_tmp);
        self->generator_tmp = NULL;
    }

    free(self->cases);
    free(self);
}

void labyrinth_calc_path(labyrinth *self, uint32 start_index) {
    if (!self->is_solve) labyrinth_solve(self, (int) start_index, FALSE);

    // clear path
    for (int cpt = 0; cpt < self->width * self->height; ++cpt) self->cases[cpt] &= ~IS_IN_PATH_MASK;

    if (!labyrinth_get_distance(self, start_index)) return;

    uint16 distance;
    uint32 index = start_index;
    direction direction;

    while (distance > 1) {
        distance = labyrinth_get_distance(self, index);
        self->cases[index] |= IS_IN_PATH_MASK;

        for (int m = 0; m < 4; m++) {
            direction = 1 << m;
            if (labyrinth_can_pass_through(self, index, direction) &&
                labyrinth_get_distance(self, labyrinth_get_directional_index(self, index, direction)) == distance - 1) {
                index = labyrinth_get_directional_index(self, index, direction);
                break;
            }
            if (m == 3) return;
        }
    }
}

BOOL labyrinth_save(const labyrinth *self, const char *filename) {

    FILE * inputFile;
    inputFile = fopen(filename, "wb");
    if (inputFile == NULL) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        return FALSE;
    }

    // 20 Bytes for header and maze size Bytes for body
    size_t data_size = 20 + self->width * self->height;
    void * data = malloc(data_size);
    ((uint32 *) data)[0] = self->width;
    ((uint32 *) data)[1] = self->height;
    ((uint32 *) data)[2] = self->start_case;
    ((uint32 *) data)[3] = self->start_case2;
    ((uint32 *) data)[4] = self->finish_case;
    for (int cpt = 0; cpt < self->width * self->height; ++cpt)
        ((char *) data)[cpt + 20] = (char ) (self->cases[cpt] & DIRECTION_MASK);

    if (fwrite(data, 1, data_size, inputFile) != data_size) {
        fprintf(stderr, "Error when writing in file %s\n", filename);
        free(data);
        fclose(inputFile);
        return FALSE;
    }

    free(data);
    fclose(inputFile);
    return TRUE;
}

