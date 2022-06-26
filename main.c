#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
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
    float * vertex_buffer;
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

typedef struct vector2f {
    float x;
    float y;
} vector2f_t;


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
    float vertex_buffer[6] = {-0.866 * r, -0.5 * r, 0.866 * r, -0.5 * r, 0 * r, 1 * r};
    returnobj->vertex_buffer = vertex_buffer;

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

void error_callback(int error, const char * description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void close_callback(GLFWwindow * window) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

char * vertexSource = "#version 150 core\nin vec2 position;void main() {gl_Position= vec4(position,0.0,1.0);}";


char * fragmentSource = "#version 150 core\nout vec4 outColor;void main() {outColor = vec4(1.0, 1.0, 1.0, 1.0);}";

int main(int argc, char ** argv)
{
    float vertices[] = {
        0.0f,  0.1f, 1.0f, 0.0f, 0.0f, // Vertex 1 (X, Y)
        0.1f, -0.1f, 0.0f, 1.0f, 0.0f,// Vertex 2 (X, Y)
        -0.1f, -0.1f, 0.0f, 0.0f, 1.0f,  // Vertex 3 (X, Y)
        0.4f,  0.5f, 1.4f, 0.0f, 0.0f, // Vertex 1 (X, Y)
        0.5f, 0.3f, 0.4f, 1.0f, 0.0f,// Vertex 2 (X, Y)
        0.3f, 0.3f, 0.4f, 0.0f, 1.0f  // Vertex 3 (X, Y)
    };

    float vert[] = {
        0.0f, 0.01f, 0.1f, -0.1f, -0.1f, -0.1f, 0.4f, 0.5f, 0.5f, 0.3f, 0.3f, 0.3f
    };

   
    //Define the triangle we will use to draw the shape
    float tri_mask[6] = {-0.866, -0.5, 0.866, 0.5, 0.0, 1.0};

    //GLFW stuff
    if (!glfwInit()) {
        printf("GLFW Initialisation Failed");
    }
    glfwSetErrorCallback(error_callback);
    GLFWwindow * window = glfwCreateWindow(1280, 720, "Fishies", NULL, NULL);
    if (!window) {
        printf("GLFW Window Creation Failed");
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    //glfwSetWindowCloseCallback(window, close_callback);
    if (!gladLoadGLLoader((GLADloadproc) (glfwGetProcAddress))) {
        printf("failed to initialise GLAD\n");
        return -1;
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSwapInterval(1);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    GLuint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    printf("%u\n", status);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    printf("%u\n", status);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindFragDataLocation(shaderProgram, 0, "outColor");

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLuint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

 //   GLuint colAttrib = glGetAttribLocation(shaderProgram, "color");
 //   glEnableVertexAttribArray(colAttrib);
 //   glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));

    const int w_size = 700;
    float light_level = 0.1;
    float sugar[w_size][w_size];

    node_t * head = init_list();
    node_t * end = get_end(head);

    for (int i = 0; i < 10; i++) {
        end = push_end(end, add_simpleobject(4,140 + rand() % 340,140 + rand() % 340,0,5,0,0), add_organism(100, 100, 0.01, true));
        end->data->vx = 0.01;
    }

    end = push_end(end, add_simpleobject(5, 590, 190, 0, -6, 0, 0), add_organism(100, 100, 0.01, true));

    int count = count_list(head);

    while (!glfwWindowShouldClose(window))
    {
        //Draw step
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glDrawArrays(GL_TRIANGLES, 0, count * 3);


        glfwSwapBuffers(window);

        //Physics step
        
        //Bounding box for collision detection, make this slightly higher than the sum of the radius of the two largest objects
        float b_box = 10.5;

        for (node_t * i_obj = head; i_obj; i_obj = i_obj->next)
        {
            for (node_t * j_obj = head; j_obj; j_obj = j_obj->next)
            {
                if (j_obj->data->y > i_obj->data->y - b_box && j_obj->data->y < i_obj->data->y + b_box && j_obj->data->x > i_obj->data->x - b_box && j_obj->data->x < i_obj->data->x + b_box) {
                    vector2f_t col_vec, col_vec_norm;
                    float psum_r = pow(i_obj->data->r + j_obj->data->r, 2);
                    float pdist = pow(i_obj->data->x - j_obj->data->x, 2) + pow(i_obj->data->y - j_obj->data->y, 2);
                    if (i_obj != j_obj) {
                    }
                    if((i_obj != j_obj) && (pdist < psum_r)) {
                        col_vec.x = i_obj->data->x - j_obj->data->x;
                        col_vec.y = i_obj->data->y - j_obj->data->y;
                        float norm_mod = sqrt(pow(col_vec.x, 2) + pow(col_vec.y, 2));
                        col_vec_norm.x = col_vec.x / norm_mod;
                        col_vec_norm.y = col_vec.y / norm_mod;
                        if (pdist < 0.01)
                        {
                            i_obj->data->vx += (((rand() % 2) * 2) - 1) * 0.1;
                            i_obj->data->vy += (((rand() % 2) * 2) - 1) * 0.1;
                        } 
                        else
                        {
                            float ia = i_obj->data->r * i_obj->data->r * M_PI; 
                            float ja = j_obj->data->r * j_obj->data->r * M_PI; 

                            // How strong should the displacement when an object is inside another be
                            float dsp = 0.02;

                            i_obj->data->x += col_vec_norm.x * dsp * (sqrt(pdist));
                            i_obj->data->y += col_vec_norm.y * dsp * (sqrt(pdist));
                            j_obj->data->x -= col_vec_norm.x * dsp * (sqrt(pdist));
                            j_obj->data->y -= col_vec_norm.y * dsp * (sqrt(pdist));
                            
                            // Loss detemines how inelastic the collisions will be
                            float loss = 0.04;
                            
                            float j_spd = sqrt(pow(j_obj->data->vx, 2) + pow(j_obj->data->vy, 2));
                            float i_spd = sqrt(pow(i_obj->data->vx, 2) + pow(i_obj->data->vy, 2));
                            
                            i_obj->data->vx += j_spd * col_vec_norm.x  * (ia / (ja + ia)) * loss;
                            i_obj->data->vy += j_spd * col_vec_norm.y  * (ia / (ja + ia)) * loss;
                            j_obj->data->vx -= i_spd * col_vec_norm.x  * (ia / (ja + ia)) * loss; 
                            j_obj->data->vy -= i_spd * col_vec_norm.y  * (ia / (ja + ia)) * loss;
                        }         
                    }
                }
            }
        }

        //Draw & apply velocities step (i know its stupid but bear with me here, im doing my best
        node_t * current = head;
        float printlist[count*6];
        int icnt = 0;
        while (current != NULL)
        {


            printlist[icnt]   =current->data->x / 700.f + 0.01;
            printlist[icnt+1] =current->data->y / 700.f - 0.01;
            printlist[icnt+2] =current->data->x / 700.f - 0.01;
            printlist[icnt+3] =current->data->y / 700.f - 0.01;
            printlist[icnt+4] =current->data->x / 700.f + 0;
            printlist[icnt+5] =current->data->y / 700.f + 0.01;
            icnt += 6;

            if (current->life != NULL) {
                if (current->life->alive == true) {
                    current->life->energy -= current->life->energy_cost;
                    current->life->energy += light_level;
                    if (current->life->energy > current->life->energy_base * 2) {
                        end = push_end(end, add_simpleobject(4, current->data->x, current->data->y, 0, 0, 0, 0), add_organism(1, 100, current->life->energy_cost, true));
                        count++;
                        current->life->energy = 1;
                    }
                }
                if (current->life->energy < 0) {current->life->alive = false;}
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


            current = current->next;
        }
    
        glBufferData(GL_ARRAY_BUFFER, sizeof(printlist), printlist, GL_DYNAMIC_DRAW);

    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
