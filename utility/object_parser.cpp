#include "object_parser.h"

namespace renderer {

std::vector<Triangle> ExtractFromObj(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Vec3> points;
    std::vector<int[3]> faces;
    std::string line;
    std::string cur;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        ss >> cur;
        if (cur == "v") {
            double v1, v2, v3;
            ss >> v1 >> v2 >> v3;
            points.emplace_back(v1, v2, v3);
        }
        if (cur == "f") {
            int a, b, c;
            ss >> a >> b >> c;
            faces.push_back({a, b, c});
        }
    }
    std::vector<Vec3> normals(points.size(), Vec3::Zero());
    for (size_t i = 0; i < faces.size(); i++) {
        Vec3 fnorm = CalculateNormalForThreeVectors(points[faces[i][0]], points[faces[i][1]], points[faces[i][2]]);
        normals[faces[i][0]] += fnorm;
        normals[faces[i][1]] += fnorm;
        normals[faces[i][2]] += fnorm;
    }
    for (size_t i = 0; i < normals.size(); i++) {
        normals[i].normalize();
    }
    std::vector<Triangle> triangles;
    for (size_t i = 0; i < faces.size(); i++) {
        triangles.emplace_back(Vertex(points[faces[i][0]], default_colour_k, normals[faces[i][0]]),
                               Vertex(points[faces[i][1]], default_colour_k, normals[faces[i][1]]),
                               Vertex(points[faces[i][2]], default_colour_k, normals[faces[i][2]]));
    }
}
}  // namespace renderer
