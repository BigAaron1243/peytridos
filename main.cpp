#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct simpleobject {
    float r;
    float x;
    float y;
    float w;
    float vx;
    float vy;
    float vw;
};

struct organism {
    float energy_base;
    float energy;
    float energy_cost;
    bool alive;
};

typedef struct node {
    struct simpleobject *data;
    struct organism *life;
    struct node *next;
} node_t;


void print_list(node_t * head) {
    node_t *current = head;

    while (current != NULL) {
        printf("%f ", current->data->r);
        current = current->next;
    }

}

int count_list(node_t * head) {
    node_t * current = head;
    int returnvalue = 0;
    while (current != NULL) {
        current = current->next;
        returnvalue++;
    }
    return returnvalue;
}

void push(node_t * head, struct simpleobject * data) {
    node_t * current = head; 
    while (current->next != NULL) 
    {
        current = current->next;
    }
    current->next = (node_t *) malloc(sizeof(node_t));
    current->next->data = data;
    current->next->next = NULL;
}

node_t * get_end(node_t * head) {
    node_t * current = head; 
    while (current->next != NULL) 
    {
        current = current->next;
    }
    return current;
}

node_t * push_end(node_t * end, struct simpleobject * data, struct organism * life) {
    end->next = (node_t *) malloc(sizeof(node_t));
    end->next->data = data;
    end->next->life = life;
    end->next->next = NULL;
    return end->next;
}


struct simpleobject * add_simpleobject(float r, float x, float y, float w, float vx, float vy, float vw) {
    struct simpleobject *returnobj = (struct simpleobject*) malloc(sizeof(struct simpleobject));

    returnobj->r = r;
    returnobj->x = x;
    returnobj->y = y;
    returnobj->w = w;
    returnobj->vx = vx;
    returnobj->vy = vy;
    returnobj->vw = vw;

    return returnobj;
}

struct organism * add_organism(float energy, float energy_base, float energy_cost, bool alive) {
    struct organism * returnobj = (struct organism *) malloc(sizeof(struct organism));

    returnobj->energy = energy;
    returnobj->energy_cost = energy_cost;
    returnobj->energy_base = energy_base;
    returnobj->alive = alive;

    return returnobj;
}

node_t * init_list() {
    node_t * head = (node_t*) malloc(sizeof(node_t)); 
    head->data = add_simpleobject(0, 0, 0, 0, 0, 0, 0);
    head->life = NULL;
    head->next = NULL;
    return head;
}

int check_collision(p_dist


template<class T> bool signTheSame(T t1, T t2, T t3) 
{
    return t1 < 0 == t2 < 0 && t1 < 0 == t3 < 0 && t2 < 0 == t3 < 0;
};

int main()
{
    const int w_size = 700;
    float light_level = 0.1;
    float sugar[w_size][w_size];

    node_t * head = init_list();
    node_t * end = get_end(head);
    sf::RenderWindow window(sf::VideoMode(700, 700), "works!");

    for (int i = 0; i < 1; i++) {
        end = push_end(end, add_simpleobject(2,140 + rand() % 340,140 + rand() % 340,0,0,0,0), add_organism(100, 100, 0.01, true));
        end->data->vx = 0.01;
    }

//          end = push_end(end, add_simpleobject(5, 590, 190, 0, -4, 0, 0), add_organism(100, 0.01, true));
//

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        //Physics step
        
        //Bounding box for collision detection, make this slightly higher than the sum of the radius of the two largest objects
        float b_box = 11;

        for (node_t * i_obj = head; i_obj; i_obj = i_obj->next)
        {
            for (node_t * j_obj = head; j_obj; j_obj = j_obj->next)
            {
                if (j_obj->data->y > i_obj->data->y - b_box && j_obj->data->y < i_obj->data->y + b_box && j_obj->data->x > i_obj->data->x - b_box && j_obj->data->x < i_obj->data->x + b_box) {
                    sf::Vector2f col_vec;
                    sf::Vector2f col_vec_norm;
                    float psum_r = pow(i_obj->data->r + j_obj->data->r, 2);
                    float pdist = pow(i_obj->data->x - j_obj->data->x, 2) + pow(i_obj->data->y - j_obj->data->y, 2);
                    if (i_obj != j_obj) {
                    }
                    if((i_obj != j_obj) && (pdist < psum_r)) {
                        col_vec = {i_obj->data->x - j_obj->data->x, i_obj->data->y - j_obj->data->y};
                        float norm_mod = sqrt(pow(col_vec.x, 2) + pow(col_vec.y, 2));
                        col_vec_norm = {col_vec.x / norm_mod, col_vec.y / norm_mod};
                        if (pdist < 0.5)
                        {
                            i_obj->data->x += (((rand() % 2) * 2) - 1);
                            i_obj->data->y += (((rand() % 2) * 2) - 1);
                        } 
                        else
                        {
                             float ia = i_obj->data->r * i_obj->data->r * M_PI; 
                            float ja = j_obj->data->r * j_obj->data->r * M_PI; 

                            // How strong should the displacement when an object is inside another be
                            float dsp = 0.01;

                            i_obj->data->x += col_vec.x * dsp * (sqrt(pdist));
                            i_obj->data->y += col_vec.y * dsp * (sqrt(pdist));
                            j_obj->data->x -= col_vec.x * dsp * (sqrt(pdist));
                            j_obj->data->y -= col_vec.y * dsp * (sqrt(pdist));
                            
                            // Loss detemines how inelastic the collisions will be
                            float loss = 2;
                            
                            i_obj->data->vx += fabs(j_obj->data->vx) * col_vec_norm.x / (2 + loss) * (ja / (ja + ia));
                            i_obj->data->vy += fabs(j_obj->data->vx) * col_vec_norm.y / (2 + loss) * (ja / (ja + ia));
                            j_obj->data->vx -= fabs(i_obj->data->vx) * col_vec_norm.x / (2 + loss) * (ia / (ja + ia));
                            j_obj->data->vy -= fabs(i_obj->data->vx) * col_vec_norm.y / (2 + loss) * (ia / (ja + ia));
                        }         
                    }
                }
            }
        }

        //Draw step (i know its stupid but bear with me here, im doing my best
        window.clear();
        node_t * current = head;
        while (current != NULL)
        {
            sf::CircleShape tempshape(current->data->r);

            if (current->life != NULL) {
                if (current->life->alive == true) {
                    current->life->energy -= current->life->energy_cost;
                    current->life->energy += light_level;
                    if (current->life->energy > current->life->energy_base * 2) {
                            end = push_end(end, add_simpleobject(2, current->data->x, current->data->y, 0, 0, 0, 0), add_organism(1, 100, current->life->energy_cost, true));
                        current->life->energy = 1;
                    }
                    if (current->life->energy < 0) {current->life->alive = false;}
                }
            }

            //Friction mu is friction coefficient
            float mu = 0.01;
                current->data->vx -= current->data->vx * mu;
                current->data->vy -= current->data->vy * mu;

            //Velocity application
            current->data->x += current->data->vx;
            current->data->y += current->data->vy;
            current->data->w += current->data->vw;

            if (current->data->x > w_size) {
                current->data->x = 0;
            }
            if (current->data->x < 0) {
                current->data->x = w_size;
            }
            if (current->data->y > w_size) {
                current->data->y = 0;
            }
            if (current->data->y < 0) {
                current->data->y = w_size;
            }

            tempshape.setPosition(current->data->x, current->data->y);
            tempshape.setOrigin(current->data->r, current->data->r);
            current = current->next;
            window.draw(tempshape);
        }
        window.display();
    }
    return 0;
}
