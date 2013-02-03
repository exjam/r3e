#ifndef CG_SHADER_HPP
#define CG_SHADER_HPP

#include <Cg/cg.h>
#include <Cg/cgGL.h>

class CgShader {
public:
	CgShader(){ InitCG(); }
	~CgShader(){}

	void checkForCgError(const char *situation){
		CGerror error;
		const char *string = cgGetLastErrorString(&error);

		if(error == CG_NO_ERROR) return;
		printf("%s: %s\n", situation, string);
		if(error == CG_COMPILER_ERROR)
			printf("%s\n", cgGetLastListing(mCgContext));
	}

	void InitCG(){
		mCgContext = cgCreateContext();
		checkForCgError("cgCreateContext()");
		cgGLSetDebugMode(CG_FALSE);
		cgSetParameterSettingMode(mCgContext, CG_DEFERRED_PARAMETER_SETTING);
		checkForCgError("cgSetParameterSettingMode()");

		mCgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
		checkForCgError("cgGLGetLatestProfile()");
		cgGLSetOptimalOptions(mCgVertexProfile);
		checkForCgError("cgGLSetOptimalOptions()");

		

		mCgVertexProgram = cgCreateProgramFromFile(mCgContext, CG_SOURCE, "1tex_skin_mesh.cg",  mCgVertexProfile, "skinMeshCG", NULL);
		checkForCgError("cgCreateProgramFromFile()");

		cgGLLoadProgram(mCgVertexProgram);
		checkForCgError("cgGLLoadProgram()");

		mBoneParameter = cgGetNamedParameter(mCgVertexProgram, "boneMatrix");
		checkForCgError("cgGetNamedParameter(boneMatrix)");
	}

	void UpdateParameters(){
		cgUpdateProgramParameters(mCgVertexProgram);
	}

	void BindVertexShader(){
		cgGLBindProgram(mCgVertexProgram);
		cgGLEnableProfile(mCgVertexProfile);
	}

	void ReleaseVertexShader(){
		cgGLDisableProfile(mCgVertexProfile);
	}

private:
	CGcontext mCgContext;
	CGprofile mCgVertexProfile;
	CGprogram mCgVertexProgram;
	CGparameter mBoneParameter;
};

#endif
