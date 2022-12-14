#pragma once
#include <framework/opengl_includes.h>
// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
DISABLE_WARNINGS_POP()
#include <span>
#include <string>
#include <vector>
#include <iostream>
#include <csignal>

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

// !!! DO NOT MODIFY THIS STRUCT !!!
// Celestial body = planet / moon / star (including the sun)
struct CelestrialBody {
    // The radius of the sphere that represents the celestial body.
    float radius { 1.0f };
    // A celestial body may spin around it's own axis. The speed at which it rotates is
    //  expressed as the spin period. The spin period is the amount of time (in hours) it
    // takes for the body to complete one full rotation.
    float spinPeriod { 0.0f };

    // A celestial body may orbit another celestial body (e.g. a moon orbiting a planet).
    // orbitAround stores the index of the object inside the bodies collection that this
    // celestial body is orbiting. The altitude describes the distance at which the object
    // is orbited (as measured from the center of both objects). The orbit period describes
    // how much time it takes for the celestial body to complete one rotation around the
    // object being orbited.
    //
    // If orbitAround is negative then this celestial body is not orbiting anything and it
    // should instead be placed at the origin (and orbitAltitude/orbitVelocity may be ignored).
    int orbitAround { -1 };
    float orbitAltitude { 0.0f };
    float orbitPeriod { 0.0f };

    // !!! YOU SHOULD IGNORE THE VARIABLES BELOW !!!
    // Only used for UI & drawing.
    std::string name;
    GLuint texture;
};

// ==========================
// =====    EXERCISE    =====
// ==========================
// Compute the positions/orientations of the solar system (list of celestial bodies) at the given time.
// Orbiting should not impact the spin of the body in any way. E.g. if the body had zero spin and was
// orbiting around another body then it should not rotate at all (only translate). If you implement
// orbit/spin correctly then the earth should always see the same side of the moon.
//
// Both spin and orbiting should be performed around the positive Y axis (glm::vec3(0, 1, 0)).
std::vector<glm::mat4> computeCelestrialBodyTransformations(std::span<const CelestrialBody> celestialBodies, float time)
{
    // For each celestial body, compute the matrix which scales the object (relative to a sphere at the origin with radius=1),
    //  translates and rotates it such that it respects the orbit / spin / scale stored in the input.
    std::vector<glm::mat4> transforms;

    std::vector<glm::vec4> translations;
    glm::vec4 translation;
    glm::mat4 rotation;
    for (auto &body : celestialBodies) {
        glm::mat4 scale = scaleMatrix(glm::vec3(body.radius, body.radius, body.radius));
        rotation = glm::rotate(glm::identity<glm::mat4>(), time * 2 * glm::pi<float>() / body.spinPeriod, glm::vec3(0, 1, 0));
        if (body.orbitAround >= 0) {
            translation = glm::vec4(body.orbitAltitude, 0.0f, 0.0f, 0.0f);
            translation = glm::rotate(glm::identity<glm::mat4>(), time * 2 * glm::pi<float>() / body.orbitPeriod, glm::vec3(0, 1, 0)) * translation;
            translation = translations[body.orbitAround] + translation;
        }
        else {
            translation = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        }

        translations.push_back(translation);
        transforms.push_back(translationMatrix(glm::vec3(translation)) * rotation * scale);
    }
    return transforms;
}