#include "object_parser.h"

#include <fstream>
#include <sstream>
#include <tuple>

namespace renderer {
std::pair<std::vector<Vec3>, std::vector<std::array<int, 3>>> ExtractPointsAndFaces(std::ifstream& file) {
    std::vector<Vec3> points;
    std::vector<std::array<int, 3>> faces;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cur;
        ss >> cur;
        if (cur == "v") {
            double v1, v2, v3;
            ss >> v1 >> v2 >> v3;
            points.emplace_back(v1, v2, v3);
        } else if (cur == "f") {
            int a, b, c;
            ss >> a >> b >> c;
            faces.push_back({a, b, c});
        }
    }
    return {points, faces};
}

std::vector<Vec3> MakeNormalsFor(const std::vector<Vec3>& points, const std::vector<std::array<int, 3>>& faces) {
    if (points.empty())
        return {};
    Vec3 sum_points = Vec3::Zero();
    for (const auto& p : points) {
        sum_points += p;
    }
    Vec3 center_of_mass = sum_points / static_cast<double>(points.size());
    std::vector<Vec3> normals(points.size(), Vec3::Zero());
    for (const auto& face : faces) {
        Vec3 p0 = points[face[0] - 1];
        Vec3 p1 = points[face[1] - 1];
        Vec3 p2 = points[face[2] - 1];
        Vec3 fnorm = CalculateOrientedNormal(p0, p1, p2);
        Vec3 face_center = (p0 + p1 + p2) / 3.0;
        normals[face[0] - 1] += fnorm;
        normals[face[1] - 1] += fnorm;
        normals[face[2] - 1] += fnorm;
    }
    for (auto& norm : normals) {
        norm.normalize();
    }
    return normals;
}

std::vector<Triangle> MakeTriangles(const std::vector<Vec3>& points, const std::vector<std::array<int, 3>>& faces,
                                     const std::vector<Vec3>& normals) {
    std::vector<Triangle> triangles;
    triangles.reserve(faces.size());
    for (const auto& face : faces) {
        triangles.emplace_back(Vertex(points[face[0] - 1], default_colour_k, normals[face[0] - 1]),
                               Vertex(points[face[1] - 1], default_colour_k, normals[face[1] - 1]),
                               Vertex(points[face[2] - 1], default_colour_k, normals[face[2] - 1]));
    }
    return triangles;
}

std::vector<Triangle> ExtractFromObj(const std::string& filename) {
    std::ifstream file(filename);
    auto [points, faces] = ExtractPointsAndFaces(file);
    std::vector<Vec3> normals = MakeNormalsFor(points, faces);
    std::vector<Triangle> triangles = MakeTriangles(points, faces, normals);

    return triangles;
}

}  // namespace renderer