#pragma once

#include <array>

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>


struct MaterialData
{
	static constexpr std::array<GLfloat, 4> DefaultDiffuse = { 0.8f, 0.8f, 0.8f, 1.f };
	static constexpr std::array<GLfloat, 4> DefualtSpecular = { 0.9f, 0.9f, 0.9f, 1.f };
	static constexpr std::array<GLfloat, 4> DefaultEmission = { 0.f, 0.f, 0.f, 0.f };
	static constexpr GLfloat DefaultShininess = 0.001f;

	std::array<GLfloat, 4> Diffuse; //r, g, b, 1.f
	std::array<GLfloat, 4> Specular; //r, g, b, 1.f
	std::array<GLfloat, 4> Emission; //r, g, b, 1.f
	GLfloat Shininiess = 1.f; // 0-1.f

	/// <summary>
	/// Material Data hold the information about the material of an object which will be used in lighting calculations.
	/// </summary>
	/// <param name="diffuse">Diffuse (colour).</param>
	/// <param name="specular">The specular highlight colour.</param>
	/// <param name="emission">Emissive colour.</param>
	/// <param name="shininess">How shiny the object is/when the specular shows up (between 0 and 1).</param>
	MaterialData(std::array<GLfloat, 4> diffuse = DefaultDiffuse, std::array<GLfloat, 4> specular = DefualtSpecular, std::array<GLfloat, 4> emission = DefaultEmission, GLfloat shininess = DefaultShininess)
	{
		this->Diffuse = diffuse;
		this->Specular = specular;
		this->Emission = emission;
		this->Shininiess = shininess;
	}

	~MaterialData() = default;

	static MaterialData GetDefault()
	{
		return MaterialData();
	}

	/// <summary>
	/// Calls relevant OpenGL commands to setup the material.
	/// </summary>
	void SetMaterial()
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Diffuse.data());
		glMaterialfv(GL_FRONT, GL_SPECULAR, Specular.data());
		glMaterialfv(GL_FRONT, GL_EMISSION, Emission.data());

		glMaterialf(GL_FRONT, GL_SHININESS, Shininiess * 128.f); //multiply by 128.f because at the moment its a value between 0 and 1
	}

	/// <summary>
	/// Calls the relevant OpenGL commands to unset the material.
	/// </summary>
	void UnsetMaterial()
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, DefaultDiffuse.data());
		glMaterialfv(GL_FRONT, GL_SPECULAR, DefualtSpecular.data());
		glMaterialfv(GL_FRONT, GL_EMISSION, DefaultEmission.data());

		glMaterialf(GL_FRONT, GL_SHININESS, DefaultShininess);
	}
};