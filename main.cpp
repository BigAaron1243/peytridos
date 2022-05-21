#include <SFML/Graphics.hpp>
#include <stdio.h>
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
    float energy;
    float energy_cost;
    bool alive;
};

typedef struct node {
    struct simpleobject *data;
    struct organism *life;
    struct node *next;
} node_t;

node_t * head = NULL;
node_t * end = NULL;

void print_list(node_t * head) {
    node_t *current = head;

    while (current != NULL) {
        printf("%f ", current->data->r);
        current = current->next;
    }

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

struct organism * add_organism(float energy, float energy_cost, bool alive) {
    struct organism * returnobj = (struct organism *) malloc(sizeof(struct organism));

    returnobj->energy = energy;
    returnobj->energy_cost = energy_cost;
    returnobj->alive = alive;

    return returnobj;
}

void init_list() {
    head = (node_t*) malloc(sizeof(node_t)); 
    head->data = add_simpleobject(0, 0, 0, 0, 0, 0, 0);
    head->life = NULL;
    head->next = NULL;
}


template<class T> bool signTheSame(T t1, T t2, T t3) 
{
    return t1 < 0 == t2 < 0 && t1 < 0 == t3 < 0 && t2 < 0 == t3 < 0;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(700, 700), "works!");
    init_list();

    end = get_end(head);
    for (int i = 0; i < 500; i++) {
      end = push_end(end, add_simpleobject(5,rand() % 340, rand() % 340,0,0,0,0), add_organism(100, 0.1, true));
    }

          end = push_end(end, add_simpleobject(5, 700, 50, 0, -1, 0, 0), add_organism(100, 0.01, true));

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

        float b_box = 15;

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
                        if (pdist == 0)
                        {
                            i_obj->data->x += (((rand() % 2) * 2) - 1) * 5;
                            i_obj->data->y += (((rand() % 2) * 2) - 1) * 5;
                        } 
                        else
                        {
                            float dsp = 0.02;
                            i_obj->data->x += col_vec.x * dsp;
                            i_obj->data->y += col_vec.y * dsp;
                            j_obj->data->x -= col_vec.x * dsp;
                            j_obj->data->y -= col_vec.y * dsp;
                            
                            float loss = 1;
                            
                            i_obj->data->vx += fabs(j_obj->data->vx) * col_vec_norm.x / (2 + loss);
                            i_obj->data->vy += fabs(j_obj->data->vx) * col_vec_norm.y / (2 + loss);
                            j_obj->data->vx -= fabs(i_obj->data->vx) * col_vec_norm.x / (2 + loss);
                            j_obj->data->vy -= fabs(i_obj->data->vx) * col_vec_norm.y / (2 + loss);
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

            tempshape.setFillColor(sf::Color::Red);
            if (current->life != NULL) {
                if (current->life->alive == true) {
                    tempshape.setFillColor(sf::Color::Green);
                    printf("%g %d ", current->life->energy, current->life->alive);
                    current->life->energy -= current->life->energy_cost;
                    if (current->life->energy < 0) {current->life->alive = false;}
                }
            }
            //Friction
            float mu = 0.0005;
                current->data->vx -= current->data->vx * mu;
                current->data->vy -= current->data->vy * mu;

            //Velocity application
            current->data->x += current->data->vx;
            current->data->y += current->data->vy;
            current->data->w += current->data->vw;

            tempshape.setPosition(current->data->x, current->data->y);
            current = current->next;
            window.draw(tempshape);
        }
        window.display();
    }
    return 0;
}
