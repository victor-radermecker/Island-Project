#include "HumanConstructions.hpp"

using namespace vcl;


void treasure_model::create_treasure_box()
{
	vec3 p0 = { 0,0,0 };
	float scale = 1.0f;
	const float L = 1.5f * scale;
	const float l = 0.65f * scale;
	const float h = 0.05f * scale;

	mesh_drawable bottom = mesh_drawable(mesh_primitive_parallelepiped(p0, { l,0,0 }, { 0,L,0 }, { 0,0,h }));
	bottom.uniform.color = { 1,1,1 };
	hierarchy_treasure.add(bottom, "bottom");

	mesh_drawable left_panel = mesh_drawable(mesh_primitive_parallelepiped(p0, { l - h,0,0 }, { 0,h,0 }, { 0,0,l - h }));
	mesh_drawable right_panel = mesh_drawable(mesh_primitive_parallelepiped(p0, { l - h,0,0 }, { 0,h,0 }, { 0,0,l - h }));
	mesh_drawable back_panel = mesh_drawable(mesh_primitive_parallelepiped(p0, { h,0,0 }, { 0,L - 2 * h,0 }, { 0,0,l - h }));
	mesh_drawable front_panel = mesh_drawable(mesh_primitive_parallelepiped(p0, { h,0,0 }, { 0,L,0 }, { 0,0,l - h }));
	mesh_drawable top = mesh_drawable(mesh_primitive_parallelepiped(p0, { l,0,0 }, { 0,L,0 }, { 0,0,h }));
	mesh_drawable money = mesh_drawable(mesh_primitive_parallelepiped(p0, { l - 2 * h,0,0 }, { 0,L - 2 * h,0 }, { 0,0,h }));

	left_panel.uniform.color = { 1,1,1 };
	right_panel.uniform.color = { 1,1,1 };
	front_panel.uniform.color = { 1,1,1 };
	back_panel.uniform.color = { 1,1,1 };
	money.uniform.color = { 1,1,0 };



	wood_texture_id = create_texture_gpu(image_load_png("scenes/3D_graphics/02_texture/assets/rocks/rock2.png"));
	front_panel.texture_id = wood_texture_id;

	hierarchy_treasure.add(left_panel, "left", "bottom", vec3(0, 0, h));
	hierarchy_treasure.add(right_panel, "right", "bottom", vec3(0, L - h, h));
	hierarchy_treasure.add(back_panel, "back", "bottom", vec3(0, h, h));
	hierarchy_treasure.add(front_panel, "front", "bottom", vec3(l - h, 0, h));
	hierarchy_treasure.add(top, "top", "back", vec3(0, -h, l - h));
	hierarchy_treasure.add(money, "money", "bottom", vec3(h, h, l - 2 * h));

	//hierarchy_treasure["bottom"].transform.translation = vec3(0, 0, 35.0f);

	hierarchy_treasure.update_local_to_global_coordinates();

}


void treasure_model::open_chest()
{
	//rotation of top
	mat3 const rot_top = rotation_from_axis_angle_mat3({ 0,1,0 }, -3.14f / 3);
	hierarchy_treasure["top"].transform.rotation = rot_top;
	hierarchy_treasure.update_local_to_global_coordinates();
}


void treasure_model::draw_treasure(std::map<std::string, GLuint>& shaders, scene_structure& scene)
{
	glUseProgram(shaders["mesh_sun"]);
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, wood_texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	draw(hierarchy_treasure, scene.camera, shaders["mesh_sun"]);
}



// Create the bridge

void treasure_model::set_bridge()
{
	bridge = create_bridge();
	bridge.uniform.transform.scaling = 0.025f;
	bridge_position = vec3(-60.f, -9.1f, 3.5f);
	bridge_texture_id = create_texture_gpu(image_load_png("scenes/3D_graphics/02_texture/assets/bridge/textures/VARANDE_AO.png"));
}


mesh treasure_model::create_bridge()
{
	mesh bridge_cpu = mesh_load_file_obj("scenes/3D_graphics/02_texture/assets/bridge/BRIDGE_LOW.obj");
	return bridge_cpu;
}


void treasure_model::draw_bridge(std::map<std::string, GLuint>& shaders, scene_structure& scene)
{
	glUseProgram(shaders["mesh_sun"]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bridge_texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	mat3 rot_bridge1 = rotation_from_axis_angle_mat3(vec3(1, 0, 0), 1.57f);

	bridge.uniform.transform.rotation = rot_bridge1;
	bridge.uniform.transform.translation = bridge_position; // set the position of bridge to 'bridge' position
	draw(bridge, scene.camera, shaders["mesh_sun"]);
}

// Create the canoe

void treasure_model::set_canoe()
{
	canoe = create_canoe();
	canoe.uniform.transform.scaling = 1.f;
	canoe_texture_id = create_texture_gpu(image_load_png("scenes/3D_graphics/02_texture/assets/canoe/wood.png"));
}


mesh treasure_model::create_canoe()
{
	mesh canoe_cpu = mesh_load_file_obj("scenes/3D_graphics/02_texture/assets/canoe/Canoe.obj");
	return canoe_cpu;
}


void treasure_model::draw_canoe(std::map<std::string, GLuint>& shaders, scene_structure& scene)
{
	glUseProgram(shaders["mesh_sun"]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, canoe_texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	canoe_position = vec3(27.f, -10.1f, 3.5f);
	mat3 rot_canoe = rotation_from_axis_angle_mat3(vec3(1, 0, 0), 1.57f);
	mat3 rot_canoe2 = rotation_from_axis_angle_mat3(vec3(0, 0, 1), -1.57f/3.5);
	canoe.uniform.transform.rotation = rot_canoe * rot_canoe2;
	canoe.uniform.transform.translation = canoe_position; 
	draw(canoe, scene.camera, shaders["mesh_sun"]);
}