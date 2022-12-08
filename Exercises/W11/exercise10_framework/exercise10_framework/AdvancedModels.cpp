#include <string>

#include "AdvancedModels.h"
#include "Helpers/Model.h"
#include "Helpers/Textures.h"

#include "AdvancedMaterial.h"


ETHTower::ETHTower()
:	Model(MAIN_DIR "/models/eth_tower.ptn")
{
	//Load Textures used for this Model
	GLTexture2D *txt;

	//textnorm_metalgrip
	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/metal_grip.png"));
	txt->createMipmaps();
	m_textures["grip"] = txt;

	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/metal_grip_norm.png"));
	txt->createMipmaps();
	m_textures["grip_normal"] = txt;

	//textnorm_darkiron
	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/metal_dark.png"));
	txt->createMipmaps();
	m_textures["dark"] = txt;

	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/metal_dark_norm.png"));
	txt->createMipmaps();
	m_textures["dark_normal"] = txt;

	//textnorm_brushedmetal
	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/brushed_metal.png"));
	txt->createMipmaps();
	m_textures["metal"] = txt;

	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/brushed_metal_norm.png"));
	txt->createMipmaps();
	m_textures["metal_normal"] = txt;

	//textnorm_vent
	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/vent.png"));
	txt->createMipmaps();
	m_textures["vent"] = txt;

	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/vent_norm.png"));
	txt->createMipmaps();
	m_textures["vent_normal"] = txt;

	//textnorm_facade
	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/facade.png"));
	txt->createMipmaps();
	m_textures["facade"] = txt;

	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/facade_norm.png"));
	txt->createMipmaps();
	m_textures["facade_normal"] = txt;

	//textnorm_concrete
	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/concrete.png"));
	txt->createMipmaps();
	m_textures["concrete"] = txt;

	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/concrete_norm.png"));
	txt->createMipmaps();
	m_textures["concrete_normal"] = txt;

	//textnorm_asphalt
	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/asphalt.png"));
	txt->createMipmaps();
	m_textures["asphalt"] = txt;

	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/asphalt_norm.png"));
	txt->createMipmaps();
	m_textures["asphalt_normal"] = txt;

	//textnorm_ventmetal
	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/vent_metal.png"));
	txt->createMipmaps();
	m_textures["ventMetal"] = txt;

	txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/vent_metal_norm.png"));
	txt->createMipmaps();
	m_textures["ventMetal_normal"] = txt;
}

void ETHTower::draw(World &world, DrawPass pass)
{
	ColoredPhongMaterial *cd_mat = dynamic_cast< ColoredPhongMaterial* >(world.getMaterial("ColoredPhong"));
	assert(cd_mat != NULL);

	TextureReflectionMaterial *tr_mat = dynamic_cast< TextureReflectionMaterial* >(world.getMaterial("TextureReflection"));
	assert(tr_mat != NULL);

	// TASK 3a)
	// Retrieve the 'TextureNormal' material from the 'world' and draw the models
	// that use this material (see commented code below)
	// ~TASK

	//untextured
	cd_mat->use(m_modelMatrix, world);
	cd_mat->setColor(Vector(1.0f, 0.8f, 0.0f));
	drawGroup(findGroup("untextured"));

	//glass_windows
	tr_mat->setDiffuseSpecCoef(0.6f, 0.5f, 0.3f);
	tr_mat->setTexture(m_textures["facade"]);
	tr_mat->use(m_modelMatrix, world);
	drawGroup(findGroup("glass_windows"));
}


Helicopter::Helicopter()
:	Model(MAIN_DIR "/models/helicopter_body.ptn")
{
	//Load Textures used for this Model

	//textnorm_metalgrip
	GLTexture2D *txt = new GLTexture2D();
	txt->loadFromImage(Image(MAIN_DIR "/textures/helicopter.png"));
	txt->createMipmaps();
	m_textures["heliTex"] = txt;
}

void Helicopter::draw(World &world, DrawPass pass)
{
	ColoredPhongMaterial *cd_mat = dynamic_cast< ColoredPhongMaterial* >(world.getMaterial("ColoredPhong"));
	TextureReflectionMaterial *tr_mat = dynamic_cast< TextureReflectionMaterial* >(world.getMaterial("TextureReflection"));

	assert(tr_mat);
	assert(cd_mat);

	cd_mat->setColor(Vector(0.7f, 0.9f, 0.7f));
	cd_mat->use(m_modelMatrix, world);

	//personback
	drawGroup(findGroup("personback"));
	//personfront
	drawGroup(findGroup("personfront"));

	tr_mat->setTexture(m_textures["heliTex"]);
	tr_mat->use(m_modelMatrix, world);
	//_mat->setColor(Vector(.7f, .7f, 0.7f));
	//weaponleft
	drawGroup(findGroup("weaponleft"));
	//weaponright
	drawGroup(findGroup("weaponright"));
	//weaponbottom
	drawGroup(findGroup("weaponbottom"));

	//body
	drawGroup(findGroup("body"));

	//window
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	tr_mat->setDiffuseSpecCoef(0.6f, 0.5f, 0.3f);
	tr_mat->setTexture(m_textures["heliTex"]);
	tr_mat->use(m_modelMatrix, world);
	drawGroup(findGroup("window"));

	glDisable(GL_BLEND);
}
