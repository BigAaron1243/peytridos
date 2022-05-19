#include <SFML/Graphics.hpp>
#include <stdio.h>

struct simpleobject {
    float r;
    float x;
    float y;
    float w;
    float vx;
    float vy;
    float vw;
    bool update;
};

typedef struct node {
    struct simpleobject *data;
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

node_t * push_end(node_t * end, struct simpleobject * data) {
    end->next = (node_t *) malloc(sizeof(node_t));
    end->next->data = data;
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

void init_list() {
    head = (node_t*) malloc(sizeof(node_t)); 
    head->data = add_simpleobject(0, 0, 0, 0, 0, 0, 0);
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
    for (int i = 0; i < 1000; i++) {
        end = push_end(end, add_simpleobject(5,rand() % 700, rand() % 700,0,0,0,0));
    }


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

        //Draw step
        window.clear();
        node_t * current = head;
        while (current->next != NULL) {
            sf::CircleShape tempshape(4);
            tempshape.setPosition(current->data->x, current->data->y);
            window.draw(tempshape);
            current = current->next;
        }
        window.display();
    }
    return 0;
}
