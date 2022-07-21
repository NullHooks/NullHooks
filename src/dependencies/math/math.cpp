#include "dependencies/utilities/csgo.hpp"

//aimtux
void math::correct_movement(vec3_t old_angles, c_usercmd* cmd, float old_forwardmove, float old_sidemove) {
	float delta_view;
	float f1;
	float f2;

	if (old_angles.y < 0.f)
		f1 = 360.0f + old_angles.y;
	else
		f1 = old_angles.y;

	if (cmd->viewangles.y < 0.0f)
		f2 = 360.0f + cmd->viewangles.y;
	else
		f2 = cmd->viewangles.y;

	if (f2 < f1)
		delta_view = abs(f2 - f1);
	else
		delta_view = 360.0f - abs(f1 - f2);

	delta_view = 360.0f - delta_view;

	cmd->forwardmove = cos(DEG2RAD(delta_view)) * old_forwardmove + cos(DEG2RAD(delta_view + 90.f)) * old_sidemove;
	cmd->sidemove = sin(DEG2RAD(delta_view)) * old_forwardmove + sin(DEG2RAD(delta_view + 90.f)) * old_sidemove;
}

vec3_t math::calculate_angle(vec3_t& in, vec3_t& out) {
	vec3_t delta = in - out;
	double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
	
	vec3_t ret;
	ret.x = (float)(atanf(delta.z / hyp) * 57.295779513082f);
	ret.y = (float)(atanf(delta.y / delta.x) * 57.295779513082f);
	ret.z = 0.0f;

	if (delta.x >= 0.0) ret.y += 180.0f;
	
	return ret;
}

vec3_t math::calculate_relative_angle(vec3_t& src, vec3_t& dst, vec3_t& viewangles) {
	vec3_t angles = calculate_angle(src, dst);

	return (angles - viewangles).normalized();
}

void math::sin_cos(float r, float* s, float* c) {
	*s = sin(r);
	*c = cos(r);
}

vec3_t math::angle_vector(vec3_t angle) {
	auto sy = sin(angle.y / 180.f * static_cast<float>(M_PI));
	auto cy = cos(angle.y / 180.f * static_cast<float>(M_PI));

	auto sp = sin(angle.x / 180.f * static_cast<float>(M_PI));
	auto cp = cos(angle.x / 180.f * static_cast<float>(M_PI));

	return vec3_t(cp * cy, cp * sy, -sp);
}

void math::transform_vector(vec3_t & a, matrix_t & b, vec3_t & out) {
	out.x = a.dot(b.mat_val[0]) + b.mat_val[0][3];
	out.y = a.dot(b.mat_val[1]) + b.mat_val[1][3];
	out.z = a.dot(b.mat_val[2]) + b.mat_val[2][3];
}

// (Degree Euler QAngle: pitch, yaw, roll)
void math::vector_angles(vec3_t & forward, vec3_t & angles) {
	if (forward.y == 0.0f && forward.x == 0.0f) {
		angles.y = 0.0f;
		angles.x = (forward.z > 0.0f) ? 270.0f : 90.0f;
	} else {
		angles.y = atan2(forward.y, forward.x) * 180 / static_cast<float>(M_PI);
		if (angles.y < 0.f) angles.y += 360.f;

		float tmp = sqrt(forward.x*forward.z + forward.y*forward.y);
		angles.x = atan2(-forward.z, tmp) * 180 / static_cast<float>(M_PI);
		if (angles.x < 0.f) angles.x += 360.f;
	}

	angles.z = 0.0f;
}

void math::angle_vectors(vec3_t & angles, vec3_t * forward, vec3_t * right, vec3_t * up) {
	float sp, sy, sr, cp, cy, cr;

	sin_cos(DEG2RAD(angles.x), &sp, &cp);
	sin_cos(DEG2RAD(angles.y), &sy, &cy);
	sin_cos(DEG2RAD(angles.z), &sr, &cr);

	if (forward) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up) {
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

void math::angle_vectors(const vec3_t& angles, vec3_t& forward) {
	float	sp, sy, cp, cy;

	math::sin_cos(DEG2RAD(angles[0]), &sp, &cp);
	math::sin_cos(DEG2RAD(angles[1]), &sy, &cy);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

vec3_t math::vector_add(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x + b.x,
		a.y + b.y,
		a.z + b.z);
}

vec3_t math::vector_subtract(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x - b.x,
		a.y - b.y,
		a.z - b.z);
}

vec3_t math::vector_multiply(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x * b.x,
		a.y * b.y,
		a.z * b.z);
}

vec3_t math::vector_divide(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x / b.x,
		a.y / b.y,
		a.z / b.z);
}

bool math::world_to_screen(const vec3_t& origin, vec3_t& screen) {

	//https://github.com/frk1/hazedumper/blob/master/config.json#L370
	static auto &viewmatrix = *(view_matrix_t *)(*(uint8_t **)(utilities::pattern_scan("client.dll", sig_viewmatrix) + 3) + 176);

	auto mul_row = [origin](size_t index) {
		return  viewmatrix[index][0] * origin.x +
			viewmatrix[index][1] * origin.y +
			viewmatrix[index][2] * origin.z +
			viewmatrix[index][3];
	};

	auto w = mul_row(3);

	if(w < 0.01f)
		return false;

	int x, y;
	interfaces::engine->get_screen_size(x, y);

	screen.x = (x / 2.0f) * (1.0f + mul_row(0) / w);
	screen.y = (y / 2.0f) * (1.0f - mul_row(1) / w);

	return true;

}

float math::get_fov(const vec3_t& view_angle, const vec3_t& aim_angle) {
	vec3_t ang, aim;

	angle_vectors(view_angle, aim);
	angle_vectors(aim_angle, ang);

	return RAD2DEG(acos(aim.dot(ang) / aim.length_sqr()));
}
