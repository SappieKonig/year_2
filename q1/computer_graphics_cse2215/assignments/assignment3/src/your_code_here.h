#pragma once
// Disable warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
DISABLE_WARNINGS_POP()
#include <iostream>
#include <limits>
#include <utility>

using Color = glm::vec3;

// !!! DO NOT MODIFY THIS STRUCT !!!
struct MaterialInformation {
    Color Kd { 0.5f, 0.5f, 0.5f }; // Diffuse coefficient per vertex.
    Color Ks { 0.5f, 0.5f, 0.5f }; // Specularity coefficient per vertex.
    float shininess = 20.0f; // Exponent for Phong and Blinn-Phong specularities per vertex.

    // Gooch shading
    float goochB, goochY, goochAlpha, goochBeta;
};
struct Positions {
    glm::vec3 vertex; // Vertex position in world space
    glm::vec3 light; // Light position in world space
    glm::vec3 camera; // Camera position in world space
};

// ==========================
// =====    EXERCISE    =====
// ==========================

// Debug function.
Color debugColor(const MaterialInformation& materialInformation, const Positions& positions, const glm::vec3& normal, const Color& lightColor)
{
    // This function you can use in any way you like!
    // E.g., for debugging purposes!
    return (normal + 1.0f) / 2.0f;

    // or random color per vertex:
    // const uint32_t hashX = std::hash<float>()(positions.vertex.x);
    // const uint32_t hashY = std::hash<float>()(positions.vertex.y);
    // const uint32_t hashZ = std::hash<float>()(positions.vertex.z);
    // return Color {
    //     (double)hashX / std::numeric_limits<uint32_t>::max(),
    //     (double)hashY / std::numeric_limits<uint32_t>::max(),
    //     (double)hashZ / std::numeric_limits<uint32_t>::max()
    // };

    // or material information:
    // return materialInformation.Kd;
}

// Standard lambertian shading: Kd * dot(N,L), clamped to zero when negative. Where L is the light vector.
Color diffuseOnly(const MaterialInformation& materialInformation, const Positions& positions, const glm::vec3& normal, const Color& lightColor)
{
    const glm::vec3 L = glm::normalize(positions.light - positions.vertex);
    const float NL = glm::dot(normal, L);
    return materialInformation.Kd * lightColor * std::max(0.0f, NL);
}


// Phong (!) Shading Specularity (http://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model)
// Follow the course, only calculate Ks pow(dot(V,R),shininess), where V is the view vector and R is the Reflection vector of the light (like in pool billard computed from the LightPos, vertexPos and normal).
// When computing specularities like this, verify that the light is on the right side of the surface, with respect to the normal
// E.g., for a plane, the light source below the plane cannot cast light on the top, hence, there can also not be any specularity.
Color phongSpecularOnly(const MaterialInformation& materialInformation, const Positions& positions, const glm::vec3& normal, const Color& lightColor)
{
        const glm::vec3 L = glm::normalize(positions.light - positions.vertex);
        const glm::vec3 V = glm::normalize(positions.camera - positions.vertex);
        const glm::vec3 R = glm::reflect(-L, normal);
        const float VR = glm::dot(V, R);
        return materialInformation.Ks * lightColor * std::pow(std::max(0.0f, VR), materialInformation.shininess);
}

// Blinn-Phong Shading Specularity (http://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model)
// Be careful!!! The pseudo code does some additional modifications to the formula seen in the course.
// Follow the course version and calculate ONLY Ks * pow(dot(N,H), shininess). The definition of H is given on the page above and in the course.
// The same test as before should be used.
Color blinnPhongSpecularOnly(const MaterialInformation& materialInformation, const Positions& positions, const glm::vec3& normal, const Color& lightColor)
{
        const glm::vec3 L = glm::normalize(positions.light - positions.vertex);
        const glm::vec3 V = glm::normalize(positions.camera - positions.vertex);
        const glm::vec3 H = glm::normalize(L + V);
        const float NH = glm::dot(normal, H);
        return materialInformation.Ks * lightColor * std::pow(std::max(0.0f, NH), materialInformation.shininess);
}

// Gooch shading model
// A shading model that uses an interpolation between warm and cool colours for increased depth perception.
Color gooch(const MaterialInformation& materialInformation, const Positions& positions, const glm::vec3& normal, const Color& lightColor)
{
        const glm::vec3 L = glm::normalize(positions.light - positions.vertex);
        const glm::vec3 V = glm::normalize(positions.camera - positions.vertex);
        const glm::vec3 H = glm::normalize(L + V);
        const glm::vec3 kd = materialInformation.Kd;
        const float NL = glm::dot(normal, L);
        const float transformed_NL = (NL + 1.0f) / 2.0f;
        const float alpha = materialInformation.goochAlpha;
        const float beta = materialInformation.goochBeta;
        const float y = materialInformation.goochY;
        const float b = materialInformation.goochB;

        const glm::vec3 blue = glm::vec3(0, 0, b);
        const glm::vec3 yellow = glm::vec3(y, y, 0);
        const glm::vec3 cool = blue + alpha * kd;
        const glm::vec3 warm = yellow + beta * kd;
        const glm::vec3 scaled_cool = cool * (1.0f - transformed_NL);
        const glm::vec3 scaled_warm = warm * transformed_NL;
        return lightColor * (scaled_cool + scaled_warm);
}

// RETURN the new light position, defined as follows:
// selectedPosition is a location on the mesh. Use this location to place the light source to cover the location as seen from cameraPosition (cover the cursor).
// Further, the light should be at a distance of 1.5 from the origin of the scene - in other words, located on a sphere of radius 1.5 around the origin.
// The selectedPos is guaranteed to always be within the sphere.
glm::vec3 userInteractionSphere(const glm::vec3& selectedPosition, const glm::vec3& cameraPosition)
{
    const glm::vec3 lightPos = selectedPosition + glm::normalize(cameraPosition - selectedPosition) * (1.5f - glm::length(selectedPosition));
    return lightPos;
}

// RETURN the new light position such that the light towards the selectedPosition is orthgonal to the normal at that location
// --- in this way, the shading boundary will be exactly at this location.
// there are several ways to do this, choose one you deem appropriate given the current light position
// no panic, I will not judge what solution you chose, as long as the above condition is met.
glm::vec3 userInteractionShadow(const glm::vec3& selectedPosition, const glm::vec3& selectedNormal, const glm::vec3& lightPos)
{
    const glm::vec3 LN_ortho = glm::normalize(glm::cross(selectedNormal, lightPos - selectedPosition));
    const glm::vec3 lightPos_ortho = selectedPosition + LN_ortho * (3.0f - glm::length(lightPos - selectedPosition));
    return lightPos_ortho;
}

// RETURN the new light position such that a specularity (highlight) will be located at selectedPosition, when viewed from cameraPos.
// Please ensure also that the light is at a distance of 1 from selectedPosition!
// There is only ONE way of doing this!
glm::vec3 userInteractionSpecular(const glm::vec3& selectedPosition, const glm::vec3& selectedNormal, const glm::vec3& cameraPosition)
{
    const glm::vec3 L = glm::normalize(selectedPosition - cameraPosition);
    const glm::vec3 R = glm::reflect(L, selectedNormal);
    const glm::vec3 lightPos = selectedPosition + R * (1.5f - glm::length(selectedPosition));
    return lightPos;
}
