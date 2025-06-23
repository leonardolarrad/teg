import teg.reflection;

int main() {
	struct vec3 {
		float x, y, z;
	};

	return teg::members_count_v<vec3> == 3;
}