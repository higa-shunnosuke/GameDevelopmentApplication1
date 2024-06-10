#include"GameObject.h"
#include"DxLib.h"

//�R���X�g���N�^
GameObject::GameObject() : 
	location(0.0f),
	box_size(0.0f),
	radian(0.0),
	image(NULL),
	sound(NULL)
{

}

//�f�X�g���N�^
GameObject::~GameObject()
{

}

//����������
void GameObject::Initialize(int object_type)
{
	type = object_type;
}

//�X�V����
void GameObject::Update()
{

}

//�`�揈��
void GameObject::Draw() const
{
#if _DEBUG
	//�����蔻��̉���

	Vector2D tl = location - (box_size / 2.0f);
	Vector2D br = location + (box_size / 2.0f);

	DrawBoxAA(tl.x, tl.y, br.x, br.y, GetColor(255, 0, 0), FALSE);
#endif
}

//�I��������
void GameObject::Finalize()
{

}

//�����蔻��ʒm����
void GameObject::OnHitCollision(GameObject* hit_object)
{
	//�����������ɍs������
}

//�폜����ʒm����
bool GameObject::Delete()
{
	//�폜���ɍs������
	bool ret = false;
	return ret;
}

//�^�C�v�擾����
int GameObject::GetType()
{
	return this->type;
}

//�ʒu���擾����
Vector2D GameObject::GetLocation() const
{
	return this->location;
}

//�ʒu���ݒ菈��
void GameObject::SetLocation(const Vector2D& location)
{
	this->location = location;
}

//�����蔻��̑傫�����擾����
Vector2D GameObject::GetBoxSize() const
{
	return box_size;
}