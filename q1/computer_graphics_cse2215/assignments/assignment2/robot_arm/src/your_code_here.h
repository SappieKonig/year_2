#pragma once
// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
DISABLE_WARNINGS_POP()
#include <array>
#include <framework/mesh.h>
#include <framework/opengl_includes.h>
#include <span>
#include <vector>

// ===============================
// =====   HELPER FUNCTIONS  =====
// ===============================
// glm::scale / glm::translate / glm::translate apply the matrices in reverse order.
// E.g. calling glm::rotate(glm::translate(inMatrix, ...), ...) will first rotate and then
// translate (reversed order in which the C++ functions are called). We provide these simple
// helpers methods that return a scale/translation/rotation matrix matrix so that you can
// decide on the multiplication order yourself.
// Example: rotationMatrix(...) * translationMatrix(...)
static glm::mat4 rotationMatrix(float angle, const glm::vec3& axis)
{
    return glm::rotate(glm::identity<glm::mat4>(), angle, axis);
}
static glm::mat4 translationMatrix(const glm::vec3& translation)
{
    return glm::translate(glm::identity<glm::mat4>(), translation);
}
static glm::mat4 scaleMatrix(const glm::vec3& scale)
{
    return glm::scale(glm::identity<glm::mat4>(), scale);
}

float x = 0.0f;

void animate()
{
    x += 0.00f;
}

// =========================
// =====   EXERCISE 1  =====
// =========================

void drawTriangle()
{
    // A simple example of a drawing function for a triangle.
    // 1) Try changing its color to red
    // 2) Try changing its vertex positions
    // 3) Add a second triangle in blue
    // 4) Add a global variable (initialized at 0), which represents the x-coordinate of the first vertex of each triangle
    // 5) Go to the function animate and increment this variable by a small value - observe the animation.


    glColor3f(1, 0, 0);
    glNormal3f(0, 0, 1);
    glBegin(GL_TRIANGLES);
    glVertex3f(x, 0, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(1, 1, 0);
    glColor3f(0, 0, 1);
    glNormal3f(1, 0, 0);
        glVertex3f(x, 1, 0);
        glVertex3f(0, 0, 1);
        glVertex3f(0, 1, 1);
    glEnd();
}

void drawUnitFace()
{
    // 1) Draw a unit quad in the x,y plane oriented along the z axis
    // 2) Make sure the orientation of the vertices is positive (counter-clock wise)
    // 3) What happens if the order is inversed?

    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(0, 1, 0);
    glEnd();
}

void drawUnitCube()
{
    // 1) Draw a unit cube using your function drawUnitFace, rely on glm::translate and glm::rotate to move the faces into position
    //     and apply the transformation using "glLoadMatrix(glm::value_ptr(nameOfMatrixVariable));"
    //    The cube should span from (0,0,0) to (1,1,1) .

    // Create a transformation that turns the unit face around the line x=0.5, z=0.5
    glm::mat4 translate_to_origin = translationMatrix(glm::vec3(-0.5f, 0.0f, -0.5f));
    glm::mat4 rotation = rotationMatrix(glm::radians(90.0f), glm::vec3(0, 1, 0));
    glm::mat4 translate_back = translationMatrix(glm::vec3(0.5, 0, 0.5));
    glm::mat4 transformation = translate_back * rotation * translate_to_origin;

    drawUnitFace();
    glLoadMatrixf(glm::value_ptr(transformation));
    drawUnitFace();
    glLoadMatrixf(glm::value_ptr(transformation * transformation));
    drawUnitFace();
    glLoadMatrixf(glm::value_ptr(transformation * transformation * transformation));
    drawUnitFace();

    // Create a transformation that turns the unit face around the line y=0.5, z=0.5
    glm::mat4 translate_to_origin2 = translationMatrix(glm::vec3(0.0f, -0.5f, -0.5f));
    glm::mat4 rotation2 = rotationMatrix(glm::radians(90.0f), glm::vec3(1, 0, 0));
    glm::mat4 translate_back2 = translationMatrix(glm::vec3(0, 0.5, 0.5));
    glm::mat4 transformation2 = translate_back2 * rotation2 * translate_to_origin2;

    glLoadMatrixf(glm::value_ptr(transformation2));
    drawUnitFace();
    glLoadMatrixf(glm::value_ptr(transformation2 * transformation2 * transformation2));
    drawUnitFace();


}

void drawUnitCube(glm::mat4 translation)
{
    // 1) Draw a unit cube using your function drawUnitFace, rely on glm::translate and glm::rotate to move the faces into position
    //     and apply the transformation using "glLoadMatrix(glm::value_ptr(nameOfMatrixVariable));"
    //    The cube should span from (0,0,0) to (1,1,1) .

    // Create a transformation that turns the unit face around the line x=0.5, z=0.5
        glm::mat4 translate_to_origin = translationMatrix(glm::vec3(-0.5f, 0.0f, -0.5f));
        glm::mat4 rotation = rotationMatrix(glm::radians(90.0f), glm::vec3(0, 1, 0));
        glm::mat4 translate_back = translationMatrix(glm::vec3(0.5, 0, 0.5));
        glm::mat4 transformation = translate_back * rotation * translate_to_origin;

        glLoadMatrixf(glm::value_ptr(translation));
        drawUnitFace();
        glLoadMatrixf(glm::value_ptr(translation * transformation));
        drawUnitFace();
        glLoadMatrixf(glm::value_ptr(translation * transformation * transformation));
        drawUnitFace();
        glLoadMatrixf(glm::value_ptr(translation * transformation * transformation * transformation));
        drawUnitFace();

    // Create a transformation that turns the unit face around the line y=0.5, z=0.5
        glm::mat4 translate_to_origin2 = translationMatrix(glm::vec3(0.0f, -0.5f, -0.5f));
        glm::mat4 rotation2 = rotationMatrix(glm::radians(90.0f), glm::vec3(1, 0, 0));
        glm::mat4 translate_back2 = translationMatrix(glm::vec3(0, 0.5, 0.5));
        glm::mat4 transformation2 = translate_back2 * rotation2 * translate_to_origin2;

        glLoadMatrixf(glm::value_ptr(translation * transformation2));
        drawUnitFace();
        glLoadMatrixf(glm::value_ptr(translation * transformation2 * transformation2 * transformation2));
        drawUnitFace();


}

struct ArmSegment {
    // Rotation around the positive x axis (glm::vec3(1,0,0)) in radians.
    float rotationX { 0.0f };
    glm::vec3 boxSize;
};

void drawArm(const ArmSegment& segment1, const ArmSegment& segment2, const ArmSegment& segment3)
{
    // Produce a three-unit arm (upper arm, forearm, hand) making use of your function drawUnitCube to define each of them.
    //1) Use the specified segments to define your arm; use glm::scale(matrix, scale) to achieve different arm length and
    //    use glm::rotate(matrix, angleInRadians, axis) and glm::translate(matrix, translation) to correctly place the elements.
    //   Modify the drawUnitCube function to take a glm::mat4 as an argument which is applied to the cube.

    glm::mat4 middle_to_origin = translationMatrix(glm::vec3(0.0f, -0.5f, 0.0f));

    glm::mat4 rotationX = rotationMatrix(segment1.rotationX, glm::vec3(1, 0, 0));
    glm::mat4 scale = scaleMatrix(segment1.boxSize);
    glm::mat4 transformation = rotationX * scale * middle_to_origin;
//    glLoadMatrixf(glm::value_ptr(transformation)); // Anything drawn after this line will be transformed by the given matrix.
    drawUnitCube(transformation);


    glm::vec4 translation_vec = transformation * glm::vec4(0, .5, 1, 1);
    glm::mat4 translation = translationMatrix(glm::vec3(translation_vec.x, translation_vec.y, translation_vec.z));
    glm::mat4 rotationX2 = rotationMatrix(segment1.rotationX + segment2.rotationX, glm::vec3(1, 0, 0));
    glm::mat4 scale2 = scaleMatrix(segment2.boxSize);
    glm::mat4 transformation2 = translation * rotationX2 * scale2 * middle_to_origin;

    drawUnitCube(transformation2);

    glm::vec4 translation_vec2 = transformation2 * glm::vec4(0, .5, 1, 1);
    glm::mat4 translation2 = translationMatrix(glm::vec3(translation_vec2.x, translation_vec2.y, translation_vec2.z));
    glm::mat4 rotationX3 = rotationMatrix(segment1.rotationX + segment2.rotationX + segment3.rotationX, glm::vec3(1, 0, 0));
    glm::mat4 scale3 = scaleMatrix(segment3.boxSize);
    glm::mat4 transformation3 = translation2 * rotationX3 * scale3 * middle_to_origin;

    drawUnitCube(transformation3);

}


void _drawMesh(Mesh mesh)
{
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < mesh.triangles.size(); i++) {
        glm::uvec3 triangle = mesh.triangles[i];
        for (int j = 0; j < 3; j++) {
            Vertex vertex = mesh.vertices[triangle[j]];
            glNormal3f(vertex.normal.x, vertex.normal.y, vertex.normal.z);
            glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
        }
    }
    glEnd();
}


std::vector<glm::vec3> _calculateNormals(Mesh mesh)
{
    std::vector<glm::vec3> normals;
    normals.reserve(mesh.vertices.size());
    for (int i = 0; i < mesh.vertices.size(); i++) {
        glm::uvec3 triangle = mesh.triangles[i];
        glm::vec3 v1 = mesh.vertices[triangle[0]].position;
        glm::vec3 v2 = mesh.vertices[triangle[1]].position;
        glm::vec3 v3 = mesh.vertices[triangle[2]].position;
        glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
        normals.push_back(normal);
    }
    return normals;
}


void _calculateVertexNormals(Mesh mesh, std::vector<glm::vec3> normals)
{
    std::vector<unsigned int> counts;
    counts.reserve(mesh.vertices.size());
    for (int i = 0; i < mesh.vertices.size(); i++) {
            counts.push_back(0);
    }
    for (int i = 0; i < mesh.triangles.size(); i++) {
        for (int j = 0; j < 3; j++) {
            mesh.vertices[mesh.triangles[i][j]].normal += normals[i];
            counts[mesh.triangles[i][j]]++;
        }
    }
    for (int i = 0; i < mesh.vertices.size(); i++) {
            mesh.vertices[i].normal /= counts[i];
    }
}


void drawMesh(Mesh mesh)
{
    //1) Use the mesh data structure; each triangle is defined with 3 consecutive indices in the mesh.triangles vector.
    //   These indices correspond to vertices stored in mesh.vertices. Provide a function that draws these triangles.
    //   Each vertex stores a position and some additional data. For this exercise you only need to look at the position.
    //
    // The exact definition of Mesh can be found in "framework/include/framework/mesh.h":
    //
    // struct Vertex {
    //     glm::vec3 position;
    //     glm::vec3 normal; // [initially zero]
    //     ...
    // };
    //
    // struct Mesh {
    //     std::vector<Vertex> vertices;
    //     std::vector<glm::uvec3> triangles;
    // };

    _drawMesh(mesh);


    //2) Compute the normals of these triangles

    auto triangle_normals = _calculateNormals(mesh);

    //3) Try computing a normal per vertex as the average of the adjacent face normals, call glNormal3f with the corresponding
    //   values before each vertex. What do you observe with respect to the lighting?

    _calculateVertexNormals(mesh, triangle_normals);

    //4) Try loading your own model (export it from Blender as a Wavefront obj) and replace the provided mesh file.

}