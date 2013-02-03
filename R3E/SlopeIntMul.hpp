/*
Filthy code but fast.
*/

#include "BoundingBox.hpp"

enum CLASSIFICATION
{ MMM, MMP, MPM, MPP, PMM, PMP, PPM, PPP, POO, MOO, OPO, OMO, OOP, OOM,
	OMM,OMP,OPM,OPP,MOM,MOP,POM,POP,MMO,MPO,PMO,PPO};

struct SlopeRay
{	
	//common variables
	float x, y, z;		// ray origin	
	float i, j, k;		// ray direction	
	float ii, ij, ik;	// inverses of direction components
	
	// ray slope
	int classification;
	float ibyj, jbyi, kbyj, jbyk, ibyk, kbyi; //slope
	float c_xy, c_xz, c_yx, c_yz, c_zx, c_zy;	
	
	void make_ray(float x, float y, float z, float i, float j, float k){
		//common variables
		this->x = x;
		this->y = y;
		this->z = z;
		this->i = i;
		this->j = j;
		this->k = k;

		this->ii = 1.0f/i;
		this->ij = 1.0f/j;
		this->ik = 1.0f/k;

		//ray slope
		this->ibyj = this->i * this->ij;
		this->jbyi = this->j * this->ii;
		this->jbyk = this->j * this->ik;
		this->kbyj = this->k * this->ij;
		this->ibyk = this->i * this->ik;
		this->kbyi = this->k * this->ii;
		this->c_xy = this->y - this->jbyi * this->x;
		this->c_xz = this->z - this->kbyi * this->x;
		this->c_yx = this->x - this->ibyj * this->y;
		this->c_yz = this->z - this->kbyj * this->y;
		this->c_zx = this->x - this->ibyk * this->z;
		this->c_zy = this->y - this->jbyk * this->z;	

		//ray slope classification
		if(i < 0)
		{
			if(j < 0)
			{
				if(k < 0)
				{
					this->classification = MMM;
				}
				else if(k > 0){
					this->classification = MMP;
				}
				else//(k >= 0)
				{
					this->classification = MMO;
				}
			}
			else//(j >= 0)
			{
				if(k < 0)
				{
					this->classification = MPM;
					if(j==0)
						this->classification = MOM;
				}
				else//(k >= 0)
				{
					if((j==0) && (k==0))
						this->classification = MOO;	
					else if(k==0)
						this->classification = MPO;
					else if(j==0)
						this->classification = MOP;
					else
						this->classification = MPP;
				}
			}
		}
		else//(i >= 0)
		{
			if(j < 0)
			{
				if(k < 0)
				{
					this->classification = PMM;
					if(i==0)
						this->classification = OMM;
				}
				else//(k >= 0)
				{				
					if((i==0) && (k==0))
						this->classification = OMO;
					else if(k==0)
						this->classification = PMO;
					else if(i==0)
						this->classification = OMP;
					else
						this->classification = PMP;
				}
			}
			else//(j >= 0)
			{
				if(k < 0)
				{
					if((i==0) && (j==0))
						this->classification = OOM;
					else if(i==0)
						this->classification = OPM;
					else if(j==0)
						this->classification = POM;
					else
						this->classification = PPM;
				}
				else//(k > 0)
				{
					if(i==0)
					{
						if(j==0)
							this->classification = OOP;
						else if(k==0)
							this->classification = OPO;
						else
							this->classification = OPP;
					}
					else
					{
						if((j==0) && (k==0))
							this->classification = POO;
						else if(j==0)
							this->classification = POP;
						else if(k==0)
							this->classification = PPO;
						else
							this->classification = PPP;
					}
				}			
			}
		}
	}
};

static bool slopeint_mul(SlopeRay *r, const BoundingBox *b, float *t){

	switch (r->classification)
	{
	case MMM:
		{
		if ((r->x < b->mMin.x) || (r->y < b->mMin.y) || (r->z < b->mMin.z)
			|| (r->jbyi * b->mMin.x - b->mMax.y + r->c_xy > 0)
			|| (r->ibyj * b->mMin.y - b->mMax.x + r->c_yx > 0)
			|| (r->jbyk * b->mMin.z - b->mMax.y + r->c_zy > 0)
			|| (r->kbyj * b->mMin.y - b->mMax.z + r->c_yz > 0)
			|| (r->kbyi * b->mMin.x - b->mMax.z + r->c_xz > 0)
			|| (r->ibyk * b->mMin.z - b->mMax.x + r->c_zx > 0)
			)
			return false;
		
		// compute the intersection distance

			*t = (b->mMax.x - r->x) * r->ii;
			float t1 = (b->mMax.y - r->y) * r->ij;
			if(t1 > *t)
				*t = t1;
			float t2 = (b->mMax.z - r->z) * r->ik;
			if(t2 > *t)
				*t = t2;

			return true;
		}


	case MMP:
		{		
		if ((r->x < b->mMin.x) || (r->y < b->mMin.y) || (r->z > b->mMax.z)
			|| (r->jbyi * b->mMin.x - b->mMax.y + r->c_xy > 0)
			|| (r->ibyj * b->mMin.y - b->mMax.x + r->c_yx > 0)
			|| (r->jbyk * b->mMax.z - b->mMax.y + r->c_zy > 0)
			|| (r->kbyj * b->mMin.y - b->mMin.z + r->c_yz < 0)
			|| (r->kbyi * b->mMin.x - b->mMin.z + r->c_xz < 0)
			|| (r->ibyk * b->mMax.z - b->mMax.x + r->c_zx > 0)
			)
			return false;
		
		*t = (b->mMax.x - r->x) * r->ii;
			float t1 = (b->mMax.y - r->y) * r->ij;
			if(t1 > *t)
				*t = t1;
			float t2 = (b->mMin.z - r->z) * r->ik;
			if(t2 > *t)
				*t = t2;
		
			return true;
		}

	case MPM:
		{		
		if ((r->x < b->mMin.x) || (r->y > b->mMax.y) || (r->z < b->mMin.z)
			|| (r->jbyi * b->mMin.x - b->mMin.y + r->c_xy < 0) 
			|| (r->ibyj * b->mMax.y - b->mMax.x + r->c_yx > 0)
			|| (r->jbyk * b->mMin.z - b->mMin.y + r->c_zy < 0) 
			|| (r->kbyj * b->mMax.y - b->mMax.z + r->c_yz > 0)
			|| (r->kbyi * b->mMin.x - b->mMax.z + r->c_xz > 0)
			|| (r->ibyk * b->mMin.z - b->mMax.x + r->c_zx > 0)
			)
			return false;
		
		*t = (b->mMax.x - r->x) * r->ii;
		float t1 = (b->mMin.y - r->y) * r->ij;
		if(t1 > *t)
			*t = t1;
		float t2 = (b->mMax.z - r->z) * r->ik;
		if(t2 > *t)
			*t = t2;

		return true;
		}

	case MPP:
		{
		if ((r->x < b->mMin.x) || (r->y > b->mMax.y) || (r->z > b->mMax.z)
			|| (r->jbyi * b->mMin.x - b->mMin.y + r->c_xy < 0) 
			|| (r->ibyj * b->mMax.y - b->mMax.x + r->c_yx > 0)
			|| (r->jbyk * b->mMax.z - b->mMin.y + r->c_zy < 0)
			|| (r->kbyj * b->mMax.y - b->mMin.z + r->c_yz < 0) 
			|| (r->kbyi * b->mMin.x - b->mMin.z + r->c_xz < 0)
			|| (r->ibyk * b->mMax.z - b->mMax.x + r->c_zx > 0)
			)
			return false;
		
		*t = (b->mMax.x - r->x) * r->ii;
			float t1 = (b->mMin.y - r->y) * r->ij;
			if(t1 > *t)
				*t = t1;
			float t2 = (b->mMin.z - r->z) * r->ik;
			if(t2 > *t)
				*t = t2;

			return true;
		}

	case PMM:
		{
		if ((r->x > b->mMax.x) || (r->y < b->mMin.y) || (r->z < b->mMin.z)
			|| (r->jbyi * b->mMax.x - b->mMax.y + r->c_xy > 0)
			|| (r->ibyj * b->mMin.y - b->mMin.x + r->c_yx < 0)
			|| (r->jbyk * b->mMin.z - b->mMax.y + r->c_zy > 0)
			|| (r->kbyj * b->mMin.y - b->mMax.z + r->c_yz > 0)
			|| (r->kbyi * b->mMax.x - b->mMax.z + r->c_xz > 0)
			|| (r->ibyk * b->mMin.z - b->mMin.x + r->c_zx < 0)
			)
			return false;
		
		*t = (b->mMin.x - r->x) * r->ii;
			float t1 = (b->mMax.y - r->y) * r->ij;
			if(t1 > *t)
				*t = t1;
			float t2 = (b->mMax.z - r->z) * r->ik;
			if(t2 > *t)
				*t = t2;

			return true;
		}
		

	case PMP:
		{
		if ((r->x > b->mMax.x) || (r->y < b->mMin.y) || (r->z > b->mMax.z)
			|| (r->jbyi * b->mMax.x - b->mMax.y + r->c_xy > 0)
			|| (r->ibyj * b->mMin.y - b->mMin.x + r->c_yx < 0)
			|| (r->jbyk * b->mMax.z - b->mMax.y + r->c_zy > 0)
			|| (r->kbyj * b->mMin.y - b->mMin.z + r->c_yz < 0)
			|| (r->kbyi * b->mMax.x - b->mMin.z + r->c_xz < 0)
			|| (r->ibyk * b->mMax.z - b->mMin.x + r->c_zx < 0)
			)
			return false;

		*t = (b->mMin.x - r->x) * r->ii;
			float t1 = (b->mMax.y - r->y) * r->ij;
			if(t1 > *t)
				*t = t1;
			float t2 = (b->mMin.z - r->z) * r->ik;
			if(t2 > *t)
				*t = t2;

			return true;
		}

	case PPM:
		{
		if ((r->x > b->mMax.x) || (r->y > b->mMax.y) || (r->z < b->mMin.z)
			|| (r->jbyi * b->mMax.x - b->mMin.y + r->c_xy < 0)
			|| (r->ibyj * b->mMax.y - b->mMin.x + r->c_yx < 0)
			|| (r->jbyk * b->mMin.z - b->mMin.y + r->c_zy < 0) 
			|| (r->kbyj * b->mMax.y - b->mMax.z + r->c_yz > 0)
			|| (r->kbyi * b->mMax.x - b->mMax.z + r->c_xz > 0)
			|| (r->ibyk * b->mMin.z - b->mMin.x + r->c_zx < 0)
			)
			return false;
		
		*t = (b->mMin.x - r->x) * r->ii;
			float t1 = (b->mMin.y - r->y) * r->ij;
			if(t1 > *t)
				*t = t1;
			float t2 = (b->mMax.z - r->z) * r->ik;
			if(t2 > *t)
				*t = t2;

			return true;
		}

	case PPP:
		{
		if ((r->x > b->mMax.x) || (r->y > b->mMax.y) || (r->z > b->mMax.z)
			|| (r->jbyi * b->mMax.x - b->mMin.y + r->c_xy < 0)
			|| (r->ibyj * b->mMax.y - b->mMin.x + r->c_yx < 0)
			|| (r->jbyk * b->mMax.z - b->mMin.y + r->c_zy < 0)
			|| (r->kbyj * b->mMax.y - b->mMin.z + r->c_yz < 0)
			|| (r->kbyi * b->mMax.x - b->mMin.z + r->c_xz < 0)
			|| (r->ibyk * b->mMax.z - b->mMin.x + r->c_zx < 0)
			)
			return false;
		
		*t = (b->mMin.x - r->x) * r->ii;
			float t1 = (b->mMin.y - r->y) * r->ij;
			if(t1 > *t)
				*t = t1;
			float t2 = (b->mMin.z - r->z) * r->ik;
			if(t2 > *t)
				*t = t2;

			return true;
		}

	case OMM:
		{
		if((r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->y < b->mMin.y) || (r->z < b->mMin.z)
			|| (r->jbyk * b->mMin.z - b->mMax.y + r->c_zy > 0)
			|| (r->kbyj * b->mMin.y - b->mMax.z + r->c_yz > 0)
			)
			return false;

		*t = (b->mMax.y - r->y) * r->ij;
		float t2 = (b->mMax.z - r->z) * r->ik;
		if(t2 > *t)
			*t = t2;

		return true;
		}

	case OMP:
		{
		if((r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->y < b->mMin.y) || (r->z > b->mMax.z)
			|| (r->jbyk * b->mMax.z - b->mMax.y + r->c_zy > 0)
			|| (r->kbyj * b->mMin.y - b->mMin.z + r->c_yz < 0)
			)
			return false;

		*t = (b->mMax.y - r->y) * r->ij;
		float t2 = (b->mMin.z - r->z) * r->ik;
		if(t2 > *t)
			*t = t2;

		return true;
		}

	case OPM:
		{
		if((r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->y > b->mMax.y) || (r->z < b->mMin.z)
			|| (r->jbyk * b->mMin.z - b->mMin.y + r->c_zy < 0) 
			|| (r->kbyj * b->mMax.y - b->mMax.z + r->c_yz > 0)
			)
			return false;

		*t = (b->mMin.y - r->y) * r->ij;		
		float t2 = (b->mMax.z - r->z) * r->ik;
		if(t2 > *t)
			*t = t2;

		return true;
		}

	case OPP:
		{
		if((r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->y > b->mMax.y) || (r->z > b->mMax.z)
			|| (r->jbyk * b->mMax.z - b->mMin.y + r->c_zy < 0)
			|| (r->kbyj * b->mMax.y - b->mMin.z + r->c_yz < 0)
			)
			return false;
		
		*t = (b->mMin.y - r->y) * r->ij;		
		float t2 = (b->mMin.z - r->z) * r->ik;
		if(t2 > *t)
			*t = t2;

		return true;
		}
		

	case MOM:
		{
		if((r->y < b->mMin.y) || (r->y > b->mMax.y)
			|| (r->x < b->mMin.x) || (r->z < b->mMin.z) 
			|| (r->kbyi * b->mMin.x - b->mMax.z + r->c_xz > 0)
			|| (r->ibyk * b->mMin.z - b->mMax.x + r->c_zx > 0)
			)
			return false;
		
		*t = (b->mMax.x - r->x) * r->ii;
		float t2 = (b->mMax.z - r->z) * r->ik;
		if(t2 > *t)
			*t = t2;

		return true;
		}
		

	case MOP:
		{
		if((r->y < b->mMin.y) || (r->y > b->mMax.y)
			|| (r->x < b->mMin.x) || (r->z > b->mMax.z) 
			|| (r->kbyi * b->mMin.x - b->mMin.z + r->c_xz < 0)
			|| (r->ibyk * b->mMax.z - b->mMax.x + r->c_zx > 0)
			)
			return false;

		*t = (b->mMax.x - r->x) * r->ii;
		float t2 = (b->mMin.z - r->z) * r->ik;
		if(t2 > *t)
			*t = t2;

		return true;
		}

	case POM:
		{
		if((r->y < b->mMin.y) || (r->y > b->mMax.y)
			|| (r->x > b->mMax.x) || (r->z < b->mMin.z)
			|| (r->kbyi * b->mMax.x - b->mMax.z + r->c_xz > 0)
			|| (r->ibyk * b->mMin.z - b->mMin.x + r->c_zx < 0)
			)
			return false;
		
		*t = (b->mMin.x - r->x) * r->ii;
		float t2 = (b->mMax.z - r->z) * r->ik;
		if(t2 > *t)
			*t = t2;

		return true;
		}
			

	case POP:
		{
		if((r->y < b->mMin.y) || (r->y > b->mMax.y)
			|| (r->x > b->mMax.x) || (r->z > b->mMax.z)
			|| (r->kbyi * b->mMax.x - b->mMin.z + r->c_xz < 0)
			|| (r->ibyk * b->mMax.z - b->mMin.x + r->c_zx < 0)
			)
			return false;

		*t = (b->mMin.x - r->x) * r->ii;
		float t2 = (b->mMin.z - r->z) * r->ik;
		if(t2 > *t)
			*t = t2;

		return true;
		}	

	case MMO:
		{
		if((r->z < b->mMin.z) || (r->z > b->mMax.z)
			|| (r->x < b->mMin.x) || (r->y < b->mMin.y)  
			|| (r->jbyi * b->mMin.x - b->mMax.y + r->c_xy > 0)
			|| (r->ibyj * b->mMin.y - b->mMax.x + r->c_yx > 0)
			)
			return false;

		*t = (b->mMax.x - r->x) * r->ii;
		float t1 = (b->mMax.y - r->y) * r->ij;
		if(t1 > *t)
			*t = t1;

		return true;
		}	

	case MPO:
		{
		if((r->z < b->mMin.z) || (r->z > b->mMax.z)
			|| (r->x < b->mMin.x) || (r->y > b->mMax.y) 
			|| (r->jbyi * b->mMin.x - b->mMin.y + r->c_xy < 0) 
			|| (r->ibyj * b->mMax.y - b->mMax.x + r->c_yx > 0)
			)
			return false;
		
		*t = (b->mMax.x - r->x) * r->ii;
		float t1 = (b->mMin.y - r->y) * r->ij;
		if(t1 > *t)
			*t = t1;
		
		return true;
		}
		

	case PMO:
		{
		if((r->z < b->mMin.z) || (r->z > b->mMax.z)
			|| (r->x > b->mMax.x) || (r->y < b->mMin.y) 
			|| (r->jbyi * b->mMax.x - b->mMax.y + r->c_xy > 0)
			|| (r->ibyj * b->mMin.y - b->mMin.x + r->c_yx < 0) 
			)
			return false;

		*t = (b->mMin.x - r->x) * r->ii;
		float t1 = (b->mMax.y - r->y) * r->ij;
		if(t1 > *t)
			*t = t1;
		
		return true;
		}

	case PPO:
		{
		if((r->z < b->mMin.z) || (r->z > b->mMax.z)
			|| (r->x > b->mMax.x) || (r->y > b->mMax.y) 
			|| (r->jbyi * b->mMax.x - b->mMin.y + r->c_xy < 0)
			|| (r->ibyj * b->mMax.y - b->mMin.x + r->c_yx < 0)
			)
			return false;
	
		*t = (b->mMin.x - r->x) * r->ii;
		float t1 = (b->mMin.y - r->y) * r->ij;
		if(t1 > *t)
			*t = t1;

		return true;
		}
		

	case MOO:
		{
		if((r->x < b->mMin.x)
			|| (r->y < b->mMin.y) || (r->y > b->mMax.y)
			|| (r->z < b->mMin.z) || (r->z > b->mMax.z)
			)
			return false;

		*t = (b->mMax.x - r->x) * r->ii;
		return true;
		}

	case POO:
		{
		if((r->x > b->mMax.x)
			|| (r->y < b->mMin.y) || (r->y > b->mMax.y)
			|| (r->z < b->mMin.z) || (r->z > b->mMax.z)
			)
			return false;

		*t = (b->mMin.x - r->x) * r->ii;
		return true;
		}

	case OMO:
		{
		if((r->y < b->mMin.y)
			|| (r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->z < b->mMin.z) || (r->z > b->mMax.z)
			)
			return false;
		
		*t = (b->mMax.y - r->y) * r->ij;
		return true;
		}

	case OPO:
		{
		if((r->y > b->mMax.y)
			|| (r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->z < b->mMin.z) || (r->z > b->mMax.z)
			)
			return false;

		*t = (b->mMin.y - r->y) * r->ij;
		return true;
		}


	case OOM:
		{
		if((r->z < b->mMin.z)
			|| (r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->y < b->mMin.y) || (r->y > b->mMax.y)
			)
			return false;

		*t = (b->mMax.z - r->z) * r->ik;
		return true;
		}

	case OOP:
		{
		if((r->z > b->mMax.z)
			|| (r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->y < b->mMin.y) || (r->y > b->mMax.y)
			)
			return false;

		*t = (b->mMin.z - r->z) * r->ik;
		return true;
		}	
	}

	return false;
}

static bool slopeint_mul(SlopeRay *r, const BoundingBox *b){

	switch (r->classification)
	{
	case MMM:
		{
		if ((r->x < b->mMin.x) || (r->y < b->mMin.y) || (r->z < b->mMin.z)
			|| (r->jbyi * b->mMin.x - b->mMax.y + r->c_xy > 0)
			|| (r->ibyj * b->mMin.y - b->mMax.x + r->c_yx > 0)
			|| (r->jbyk * b->mMin.z - b->mMax.y + r->c_zy > 0)
			|| (r->kbyj * b->mMin.y - b->mMax.z + r->c_yz > 0)
			|| (r->kbyi * b->mMin.x - b->mMax.z + r->c_xz > 0)
			|| (r->ibyk * b->mMin.z - b->mMax.x + r->c_zx > 0)
			)
			return false;

			return true;
		}


	case MMP:
		{		
		if ((r->x < b->mMin.x) || (r->y < b->mMin.y) || (r->z > b->mMax.z)
			|| (r->jbyi * b->mMin.x - b->mMax.y + r->c_xy > 0)
			|| (r->ibyj * b->mMin.y - b->mMax.x + r->c_yx > 0)
			|| (r->jbyk * b->mMax.z - b->mMax.y + r->c_zy > 0)
			|| (r->kbyj * b->mMin.y - b->mMin.z + r->c_yz < 0)
			|| (r->kbyi * b->mMin.x - b->mMin.z + r->c_xz < 0)
			|| (r->ibyk * b->mMax.z - b->mMax.x + r->c_zx > 0)
			)
			return false;

			return true;
		}

	case MPM:
		{		
		if ((r->x < b->mMin.x) || (r->y > b->mMax.y) || (r->z < b->mMin.z)
			|| (r->jbyi * b->mMin.x - b->mMin.y + r->c_xy < 0) 
			|| (r->ibyj * b->mMax.y - b->mMax.x + r->c_yx > 0)
			|| (r->jbyk * b->mMin.z - b->mMin.y + r->c_zy < 0) 
			|| (r->kbyj * b->mMax.y - b->mMax.z + r->c_yz > 0)
			|| (r->kbyi * b->mMin.x - b->mMax.z + r->c_xz > 0)
			|| (r->ibyk * b->mMin.z - b->mMax.x + r->c_zx > 0)
			)
			return false;

		return true;
		}

	case MPP:
		{
		if ((r->x < b->mMin.x) || (r->y > b->mMax.y) || (r->z > b->mMax.z)
			|| (r->jbyi * b->mMin.x - b->mMin.y + r->c_xy < 0) 
			|| (r->ibyj * b->mMax.y - b->mMax.x + r->c_yx > 0)
			|| (r->jbyk * b->mMax.z - b->mMin.y + r->c_zy < 0)
			|| (r->kbyj * b->mMax.y - b->mMin.z + r->c_yz < 0) 
			|| (r->kbyi * b->mMin.x - b->mMin.z + r->c_xz < 0)
			|| (r->ibyk * b->mMax.z - b->mMax.x + r->c_zx > 0)
			)
			return false;

			return true;
		}

	case PMM:
		{
		if ((r->x > b->mMax.x) || (r->y < b->mMin.y) || (r->z < b->mMin.z)
			|| (r->jbyi * b->mMax.x - b->mMax.y + r->c_xy > 0)
			|| (r->ibyj * b->mMin.y - b->mMin.x + r->c_yx < 0)
			|| (r->jbyk * b->mMin.z - b->mMax.y + r->c_zy > 0)
			|| (r->kbyj * b->mMin.y - b->mMax.z + r->c_yz > 0)
			|| (r->kbyi * b->mMax.x - b->mMax.z + r->c_xz > 0)
			|| (r->ibyk * b->mMin.z - b->mMin.x + r->c_zx < 0)
			)
			return false;
		
			return true;
		}
		

	case PMP:
		{
		if ((r->x > b->mMax.x) || (r->y < b->mMin.y) || (r->z > b->mMax.z)
			|| (r->jbyi * b->mMax.x - b->mMax.y + r->c_xy > 0)
			|| (r->ibyj * b->mMin.y - b->mMin.x + r->c_yx < 0)
			|| (r->jbyk * b->mMax.z - b->mMax.y + r->c_zy > 0)
			|| (r->kbyj * b->mMin.y - b->mMin.z + r->c_yz < 0)
			|| (r->kbyi * b->mMax.x - b->mMin.z + r->c_xz < 0)
			|| (r->ibyk * b->mMax.z - b->mMin.x + r->c_zx < 0)
			)
			return false;

			return true;
		}

	case PPM:
		{
		if ((r->x > b->mMax.x) || (r->y > b->mMax.y) || (r->z < b->mMin.z)
			|| (r->jbyi * b->mMax.x - b->mMin.y + r->c_xy < 0)
			|| (r->ibyj * b->mMax.y - b->mMin.x + r->c_yx < 0)
			|| (r->jbyk * b->mMin.z - b->mMin.y + r->c_zy < 0) 
			|| (r->kbyj * b->mMax.y - b->mMax.z + r->c_yz > 0)
			|| (r->kbyi * b->mMax.x - b->mMax.z + r->c_xz > 0)
			|| (r->ibyk * b->mMin.z - b->mMin.x + r->c_zx < 0)
			)
			return false;
		
			return true;
		}

	case PPP:
		{
		if ((r->x > b->mMax.x) || (r->y > b->mMax.y) || (r->z > b->mMax.z)
			|| (r->jbyi * b->mMax.x - b->mMin.y + r->c_xy < 0)
			|| (r->ibyj * b->mMax.y - b->mMin.x + r->c_yx < 0)
			|| (r->jbyk * b->mMax.z - b->mMin.y + r->c_zy < 0)
			|| (r->kbyj * b->mMax.y - b->mMin.z + r->c_yz < 0)
			|| (r->kbyi * b->mMax.x - b->mMin.z + r->c_xz < 0)
			|| (r->ibyk * b->mMax.z - b->mMin.x + r->c_zx < 0)
			)
			return false;

			return true;
		}

	case OMM:
		{
		if((r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->y < b->mMin.y) || (r->z < b->mMin.z)
			|| (r->jbyk * b->mMin.z - b->mMax.y + r->c_zy > 0)
			|| (r->kbyj * b->mMin.y - b->mMax.z + r->c_yz > 0)
			)
			return false;

		return true;
		}

	case OMP:
		{
		if((r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->y < b->mMin.y) || (r->z > b->mMax.z)
			|| (r->jbyk * b->mMax.z - b->mMax.y + r->c_zy > 0)
			|| (r->kbyj * b->mMin.y - b->mMin.z + r->c_yz < 0)
			)
			return false;

		return true;
		}

	case OPM:
		{
		if((r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->y > b->mMax.y) || (r->z < b->mMin.z)
			|| (r->jbyk * b->mMin.z - b->mMin.y + r->c_zy < 0) 
			|| (r->kbyj * b->mMax.y - b->mMax.z + r->c_yz > 0)
			)
			return false;

		return true;
		}

	case OPP:
		{
		if((r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->y > b->mMax.y) || (r->z > b->mMax.z)
			|| (r->jbyk * b->mMax.z - b->mMin.y + r->c_zy < 0)
			|| (r->kbyj * b->mMax.y - b->mMin.z + r->c_yz < 0)
			)
			return false;

		return true;
		}
		

	case MOM:
		{
		if((r->y < b->mMin.y) || (r->y > b->mMax.y)
			|| (r->x < b->mMin.x) || (r->z < b->mMin.z) 
			|| (r->kbyi * b->mMin.x - b->mMax.z + r->c_xz > 0)
			|| (r->ibyk * b->mMin.z - b->mMax.x + r->c_zx > 0)
			)
			return false;
		
		return true;
		}
		

	case MOP:
		{
		if((r->y < b->mMin.y) || (r->y > b->mMax.y)
			|| (r->x < b->mMin.x) || (r->z > b->mMax.z) 
			|| (r->kbyi * b->mMin.x - b->mMin.z + r->c_xz < 0)
			|| (r->ibyk * b->mMax.z - b->mMax.x + r->c_zx > 0)
			)
			return false;

		return true;
		}

	case POM:
		{
		if((r->y < b->mMin.y) || (r->y > b->mMax.y)
			|| (r->x > b->mMax.x) || (r->z < b->mMin.z)
			|| (r->kbyi * b->mMax.x - b->mMax.z + r->c_xz > 0)
			|| (r->ibyk * b->mMin.z - b->mMin.x + r->c_zx < 0)
			)
			return false;

		return true;
		}
			

	case POP:
		{
		if((r->y < b->mMin.y) || (r->y > b->mMax.y)
			|| (r->x > b->mMax.x) || (r->z > b->mMax.z)
			|| (r->kbyi * b->mMax.x - b->mMin.z + r->c_xz < 0)
			|| (r->ibyk * b->mMax.z - b->mMin.x + r->c_zx < 0)
			)
			return false;

		return true;
		}	

	case MMO:
		{
		if((r->z < b->mMin.z) || (r->z > b->mMax.z)
			|| (r->x < b->mMin.x) || (r->y < b->mMin.y)  
			|| (r->jbyi * b->mMin.x - b->mMax.y + r->c_xy > 0)
			|| (r->ibyj * b->mMin.y - b->mMax.x + r->c_yx > 0)
			)
			return false;

		return true;
		}	

	case MPO:
		{
		if((r->z < b->mMin.z) || (r->z > b->mMax.z)
			|| (r->x < b->mMin.x) || (r->y > b->mMax.y) 
			|| (r->jbyi * b->mMin.x - b->mMin.y + r->c_xy < 0) 
			|| (r->ibyj * b->mMax.y - b->mMax.x + r->c_yx > 0)
			)
			return false;
		
		return true;
		}
		

	case PMO:
		{
		if((r->z < b->mMin.z) || (r->z > b->mMax.z)
			|| (r->x > b->mMax.x) || (r->y < b->mMin.y) 
			|| (r->jbyi * b->mMax.x - b->mMax.y + r->c_xy > 0)
			|| (r->ibyj * b->mMin.y - b->mMin.x + r->c_yx < 0) 
			)
			return false;
		
		return true;
		}

	case PPO:
		{
		if((r->z < b->mMin.z) || (r->z > b->mMax.z)
			|| (r->x > b->mMax.x) || (r->y > b->mMax.y) 
			|| (r->jbyi * b->mMax.x - b->mMin.y + r->c_xy < 0)
			|| (r->ibyj * b->mMax.y - b->mMin.x + r->c_yx < 0)
			)
			return false;

		return true;
		}
		

	case MOO:
		{
		if((r->x < b->mMin.x)
			|| (r->y < b->mMin.y) || (r->y > b->mMax.y)
			|| (r->z < b->mMin.z) || (r->z > b->mMax.z)
			)
			return false;

		return true;
		}

	case POO:
		{
		if((r->x > b->mMax.x)
			|| (r->y < b->mMin.y) || (r->y > b->mMax.y)
			|| (r->z < b->mMin.z) || (r->z > b->mMax.z)
			)
			return false;

		return true;
		}

	case OMO:
		{
		if((r->y < b->mMin.y)
			|| (r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->z < b->mMin.z) || (r->z > b->mMax.z)
			)
			return false;
		
		return true;
		}

	case OPO:
		{
		if((r->y > b->mMax.y)
			|| (r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->z < b->mMin.z) || (r->z > b->mMax.z)
			)
			return false;

		return true;
		}


	case OOM:
		{
		if((r->z < b->mMin.z)
			|| (r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->y < b->mMin.y) || (r->y > b->mMax.y)
			)
			return false;

		return true;
		}

	case OOP:
		{
		if((r->z > b->mMax.z)
			|| (r->x < b->mMin.x) || (r->x > b->mMax.x)
			|| (r->y < b->mMin.y) || (r->y > b->mMax.y)
			)
			return false;

		return true;
		}	
	}

	return false;
}
