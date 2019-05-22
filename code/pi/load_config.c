#include <load_config.h>

static rf24_pa_dbm_e cnfg_get_power_level(YAML::Node *n);
static rf24_datarate_e cnfg_get_data_rate(YAML::Node *n);
static rf24_crclength_e cnfg_get_crc_length(YAML::Node *n);
static uint8_t cnfg_get_channel(YAML::Node *n);
static address_e cnfg_get_address(YAML::Node *n);
static uint32_t cnfg_get_zoom_in(YAML::Node *n);
static uint32_t cnfg_get_zoom_out(YAML::Node *n);
static uint32_t cnfg_get_focus_in(YAML::Node *n);
static uint32_t cnfg_get_focus_out(YAML::Node *n);
static color_e cnfg_get_color(YAML::Node *n);
static uint8_t cnfg_get_id(YAML::Node *n);


node_vect* load_node_config(std::string filepath)
{
  node_prop np;
  node_vect *nv = new node_vect;

  // load and parse file
  YAML::Node base = YAML::LoadFile(filepath);
  base = base["camera_nodes"];

  for (auto it = base.begin(); it != base.end(); ++it) {

  }

}


void dump_node(node_vect *nv);
