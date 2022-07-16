#pragma once

#include "../include/Bullet.hpp"
#include "../include/Sprite.hpp"
#include "../include/GameObject.hpp"
#include "../include/Vec2.hpp"
#include "../include/Game.hpp"
#include <string>
#include <cmath>

Bullet::Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance, Sprite* sprite)
    : Component(associated)
{
	this->speed = Vec2(speed).Rotate(angle);
	this->damage = damage;
	this->distanceLeft = maxDistance;
	sprite->angleDeg = angle*(180.0/M_PI);
	associated.box.w = sprite->GetWidth();
	associated.box.h = sprite->GetHeight();    
	associated.AddComponent(sprite);
}

void Bullet::Update(float dt) 
{
	Vec2 dst = this->speed*dt;
	const float mgdst = dst.mag();
	if(mgdst >= distanceLeft)
	{
		dst = this->speed*(distanceLeft/mgdst);	
		this->associated.RequestDelete();
	}
	this->associated.box.x += dst.x;
	this->associated.box.y += dst.y;
	this->distanceLeft -= mgdst;
}

void Bullet::Render() {}

bool Bullet::Is(const std::string& type) { return type == "Bullet"; }

int Bullet::GetDamage() { return this->damage; }