//
//  Display a rotating cube
//
#define GL_SILENCE_DEPRECATION

#include "Angel.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>


typedef vec4  color4;
typedef vec4  point4;

// for moving the object
float projection = 5.0f;
float gravity = 0.01;
GLfloat xPos;
GLfloat yPos;
GLfloat xVel;
GLfloat yVel;
GLfloat previousYVel;
bool firstJump = true;

// for reshaping object according to the width/height of the screen
GLfloat originalWidth = 720;
GLfloat originalHeight = 720;
GLfloat currentWidth = 720;
GLfloat currentHeight = 720;
float scale = originalWidth/originalHeight;
float scaleOrtho = originalWidth/originalHeight;
GLfloat widthScale = 1.0;

// for VAOs and VBOs
GLuint vaoCube;
GLuint vaoSphere;
GLuint vaoBunny;
GLuint sphere_buf;
GLuint cube_buf;
GLuint bunny_buf;

// for color
int colorChange = 1;
vec4 colorWhite = {1.0, 1.0, 1.0, 1.0};
vec4 colorDarkslategray = {47.0/255, 79.0/255, 79.0/255, 1.0};
color4 white = color4( 1.0, 1.0, 1.0, 1.0 );
GLuint fColorUni;

// determining the type of the object
int type = 0; // 0 = cube  1 = sphere 2 = bunny
int drawingMode = 1; // 1 = solid -1 = wireframe


// from SpinCube example
const int NumVertices_cube = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

//  from texbook Appendix 7
const int NumTimesToSubdivide = 5;
const int NumTriangles = 4096;
// (4 faces)^(NumTimesToSubdivide + 1)
const int NumVertices_sphere = 3 * NumTriangles;
typedef Angel::vec4 point4;
typedef Angel::vec4 color4;
point4 points_sphere[NumVertices_sphere];
vec3   normals_sphere[NumVertices_sphere];
color4 colors_sphere[NumVertices_sphere];
int Index = 0;

//Bunny datas
int numberOfCoordinates_bunny;
int numberOfIndicies_bunny;

vec3 *coordinates;
GLuint *indicies;


// Vertices of a unit cube centered at origin, sides aligned with axes from SpinCube example
point4 points[8] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
};

// RGBA olors
color4 colors[8] = {
    white,
    white,
    white,
    white,
    white,
    white,
    white,
    white
};


// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;


//---------------------------------------------------------------------
//
// from texbook App 7 to draw sphere
//
void
triangle( const point4& a, const point4& b, const point4& c )
{
    vec3  normal = normalize( cross(b - a, c - b) );
    normals_sphere[Index] = normal;  points_sphere[Index] = a;  Index++;
    normals_sphere[Index] = normal;  points_sphere[Index] = b;  Index++;
    normals_sphere[Index] = normal;  points_sphere[Index] = c;  Index++;
}
//----------------------------------------------------------------------
point4
unit( const point4& p )
{
    float len = p.x*p.x + p.y*p.y + p.z*p.z;
    point4 t;
    if ( len > DivideByZeroTolerance ) {
        t = p / sqrt(len);
t.w = 1.0; }
return t; }

void
divide_triangle( const point4& a, const point4& b,
       const point4& c, int count )
{
    if ( count > 0 ) {
        point4 v1 = unit( a + b );
        point4 v2 = unit( a + c );
        point4 v3 = unit( b + c );
        divide_triangle(  a, v1, v2, count - 1 );
        divide_triangle(  c, v2, v3, count - 1 );
        divide_triangle(  b, v3, v1, count - 1 );
        divide_triangle( v1, v3, v2, count - 1 );
}
else {
        triangle( a, b, c );
    }
}
void
tetrahedron( int count )
{
    point4 v[4] = {
        vec4( 0.0, 0.0, 1.0, 1.0 ),
        vec4( 0.0, 0.942809, -0.333333, 1.0 ),
        vec4( -0.816497, -0.471405, -0.333333, 1.0 ),
        vec4( 0.816497, -0.471405, -0.333333, 1.0 )
};
    divide_triangle( v[0], v[1], v[2], count );
    divide_triangle( v[3], v[2], v[1], count );
    divide_triangle( v[0], v[3], v[1], count );
    divide_triangle( v[0], v[2], v[3], count );
}


// to load the file of the bunny
void loadOffFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line); // OFF
    std::getline(file, line);
    std::istringstream iss(line);

    
    iss >> numberOfCoordinates_bunny >> numberOfIndicies_bunny;
    
    coordinates = new vec3[numberOfCoordinates_bunny];
    indicies = new GLuint[numberOfIndicies_bunny * 3];
    
    //reading the coordinates
    for (int i = 0; i < numberOfCoordinates_bunny; ++i) {
        std::getline(file, line);
        std::istringstream iss(line);
        iss >> coordinates[i].x >> coordinates[i].y >> coordinates[i].z;
    }
    
    //reading the indicies
    int temp = 0;
       for (int i = 0; i < numberOfIndicies_bunny * 3; i +=3) {
           getline(file, line);
           std::istringstream iss(line);
           iss >> temp >> indicies[i] >> indicies[i+1] >> indicies[i+2];
           
       }
}


void init()
{
    // draw the sphere from Appendix 7
    tetrahedron( NumTimesToSubdivide );
    
    // default color white
    colorChange = 1;
    
    // initialize the objects location and the gravity according to the inital screen and projection
    xPos = -projection;
    yPos = 5;
    xVel = 5 * gravity;
    yVel = gravity;
    previousYVel = yVel;
  
    // Load and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

    // --------------------CUBE------------------------------------------------------
    // Create a VAO for cube (from SpinCube)
    glGenVertexArrays( 1, &vaoCube );
    glBindVertexArray( vaoCube );
    // Create and initialize a buffer object for cube
    glGenBuffers( 1, &cube_buf );
    glBindBuffer( GL_ARRAY_BUFFER, cube_buf );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );
    
    GLuint cube_indices[] = {
            0, 1, 2,
            2, 3, 0,
            1, 5, 6,
            6, 2, 1,
            7, 6, 5,
            5, 4, 7,
            4, 0, 3,
            3, 7, 4,
            4, 5, 1,
            1, 0, 4,
            3, 2, 6,
            6, 7, 3
        };
    
    // Create and initialize an index buffer object for cube (from SpinCube)
    GLuint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
    
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    // get the fragment shader's color location to change the color of the object  using shader
    fColorUni = glGetUniformLocation( program, "color" );
    
    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );
    glUniform4fv(fColorUni, 1, colorWhite);
    // --------------------------------------------------------------------------------
    
    // --------------------SPHERE------------------------------------------------------
    // Create a vertex array object for sphere (from Appendix 7)
    glGenVertexArrays( 1, &vaoSphere );
    glBindVertexArray( vaoSphere );
    // Create and initialize a buffer object (from Appendix 7)
    
    glGenBuffers( 1, &sphere_buf );
    glBindBuffer( GL_ARRAY_BUFFER, sphere_buf );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points_sphere) + sizeof(normals_sphere), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points_sphere), points_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points_sphere),sizeof(normals_sphere), normals_sphere );
    // set up vertex arrays
    GLuint vPosition_sphere = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition_sphere );
    glVertexAttribPointer( vPosition_sphere, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    GLuint vNormal_sphere = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal_sphere );
    glVertexAttribPointer( vNormal_sphere, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points_sphere)) );
    // --------------------------------------------------------------------------------
    
    // --------------------BUNNY------------------------------------------------------
    
    glGenVertexArrays( 1, &vaoBunny );
    glBindVertexArray( vaoBunny );
    // Create and initialize a buffer object for cube
    glGenBuffers( 1, &bunny_buf );
    glBindBuffer( GL_ARRAY_BUFFER, bunny_buf );

    glBufferData( GL_ARRAY_BUFFER, sizeof(coordinates) * numberOfCoordinates_bunny, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(coordinates) * numberOfCoordinates_bunny, coordinates );
    
    GLuint index_buffer_bunny;
    glGenBuffers(1, &index_buffer_bunny);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_bunny);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies) * numberOfIndicies_bunny, indicies, GL_STATIC_DRAW);
    

    GLuint vPosition_bunny = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition_bunny );
    glVertexAttribPointer( vPosition_bunny, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    // --------------------------------------------------------------------------------
    
    // Retrieve transformation uniform variable locations
    ModelView = glGetUniformLocation( program, "ModelView" );
    Projection = glGetUniformLocation( program, "Projection" );
    
    // initialize a projection using Ortho
    mat4  projection;
    projection = Ortho(-5, 5, -5, 5, -5, 5);
    glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );
    glUseProgram( program );
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //  Generate the model-view matrix
    const vec3 displacement( xPos, yPos, 0.0 );
    mat4   model_view;
    
    //Cube
    if (type == 0) {
        //draw the cube from its VAO
        glBindVertexArray(vaoCube);
        glDrawElements(GL_TRIANGLES, NumVertices_cube, GL_UNSIGNED_INT, 0);
        //rotate the cube
        model_view = (Translate(displacement) * Scale(scale, scale, scale) * RotateX( 15 ) * RotateY( 15 ) * RotateZ( 15 ));
    }
    //Sphere
    else if (type == 1)  {
        //draw the sphere from its VAO
        glBindVertexArray(vaoSphere);
        //draw the sphere using glDrawArray (from Appendix 7)
        glDrawArrays( GL_TRIANGLES, 0, NumVertices_sphere );
        model_view = (Translate(displacement) * Scale(scale*0.6f, scale*0.6f, scale*0.6f));
    }
    //Bunny
    else {
        glBindVertexArray(vaoBunny);
        glDrawElements(GL_TRIANGLES, numberOfCoordinates_bunny * 3, GL_UNSIGNED_INT, 0);
        model_view = (Translate(displacement) * Scale(scale*0.04f, scale*0.04f, scale*0.04f)) * RotateY( 90 ) * RotateX( 270 );
    }
    //Solid drawing mode
    if(drawingMode == 1) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    //Wireframe drawing mode
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view );
    glFlush();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); //may not need this since the default is usually the window size
    
    // Set projection matrix
    mat4  projection;
    if (width <= height){
        //if the height of the window is larger then we adjust the bottom and top projection
        projection = Ortho(-5.0, 5.0, -5.0 * (GLfloat) height / (GLfloat) width,
                           5.0 * (GLfloat) height / (GLfloat) width, -5, 5);
        // adjusting the movement of the object according to the ortho projection
        xVel = width/originalWidth * gravity * 5;
        yVel *= height/originalHeight;
    }
    else{
        projection = Ortho(-5.0 * (GLfloat) width / (GLfloat) height, 5.0 *
                             (GLfloat) width / (GLfloat) height, -5.0, 5.0, -5, 5);
        // adjusting the movement of the object according to the ortho projection
        xVel = width/originalWidth * gravity * 5;
        yVel *= height/originalHeight;
    }
    currentWidth = width;
    currentHeight = height;
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_RELEASE) {
        switch( key ) {
            case GLFW_KEY_ESCAPE: case GLFW_KEY_Q:
                exit( EXIT_SUCCESS );
                break;
            case GLFW_KEY_I: case 39:
                
                // to adjust the position of the object to the top left corner of the window (according to the ortho projection
                if ( currentWidth > currentHeight) {
                    // changing the position and the velocity of the x axis
                    xPos = - 5.0 * (GLfloat) currentWidth / (GLfloat) currentHeight;
                    yPos = 5;
                    gravity = 0.01f;
                    xVel = 5 *gravity * (GLfloat) currentWidth / ((GLfloat) currentHeight);
                }else {
                    // changing the position and the velocity of the y axis
                    xPos = - 5.0;
                    gravity = 0.01f * (GLfloat) currentHeight / (GLfloat) currentWidth;
                    yPos = 5 * (GLfloat) currentHeight / (GLfloat) currentWidth;
                    xVel = 5 *0.01;
                }
                yVel=gravity;
                previousYVel = yVel;
                firstJump = true;
                break;
            case GLFW_KEY_C:
                colorChange *= -1;
                //used fragment shader to changed the color of the object
                if (colorChange == 1){
                    glUniform4fv(fColorUni, 1, colorWhite);
                } else {
                    glUniform4fv(fColorUni, 1, colorDarkslategray);
                }
                break;
                
            case GLFW_KEY_H:
                std::cout << "Input Controls:" << std::endl;
                std::cout << "i -- initialize the pose (top left corner of the window)" << std::endl;
                std::cout << "c -- switch between two colors (of your choice), which is used to draw lines or triangles."<< std::endl;
                std::cout << "h -- help; print explanation of your input control (simply to the command line)"<< std::endl;
                std::cout << "q -- quit (exit) the program" << std::endl;
                break;
        }
    }
    
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if ( action == GLFW_PRESS ) {
        switch( button ) {
            case GLFW_MOUSE_BUTTON_RIGHT:
                // changin the type of the object
                type += 1;
                type = type % 3;
                break;
            
            case GLFW_MOUSE_BUTTON_LEFT:
                drawingMode *=-1;
                break;
        }
    }
}


void moveObject(GLfloat offlen, GLfloat heightScale) {
    
    // move the object
    yVel -= gravity;
    xPos += xVel;
    yPos += yVel;
    
    // condition to check if the object touches the ground
    if ((yPos <= (-projection * heightScale) + offlen)) {
    
        if(firstJump){
            previousYVel = yVel;
            firstJump = false;
        }
        // to make the object stop
        if (yVel < previousYVel){
            yPos = -projection * heightScale + offlen;
            yVel = 0;
            xVel = 0;
            gravity = 0;
        }
        previousYVel = yVel;
        yVel *= -1.0;
        gravity *= 1.4;
    }
}

void update( void )
{
    // to update object's location (according to the ortho projection)
    if (currentHeight>currentWidth) moveObject(scaleOrtho/2,(GLfloat) currentHeight / (GLfloat) currentWidth);
    else moveObject(scaleOrtho/2 ,1.0);
}


int main()
{
    // to load the coordinates and the indices of the bunny
    loadOffFile("bunny.off");
    
    if (!glfwInit())
            exit(EXIT_FAILURE);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(originalWidth, originalHeight, "HW1", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    if (!window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    init();

    double frameRate = 120, currentTime, previousTime = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        if (currentTime - previousTime >= 1/frameRate){
            previousTime = currentTime;
            update();
        }
        
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

