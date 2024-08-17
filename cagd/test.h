// ::::::::::::::
// aitken.c
// ::::::::::::::
float aitken(degree,coeff,t)
/*
     uses  Aitken's algorithm to compute one coordinate
     value of a  Lagrange curve. Has to be called 
     for each coordinate  (x,y, and/or z) of data set.
     Input:   degree: degree of curve.
              coeff:  array with coordinates to be interpolated.
              t:      parameter value where to interpolate.
      Output: coordinate value.

      Note: we  assume a uniform knot sequence!
*/
float coeff[];
float t;
int degree; 
{
    int r,i;
    float t1;
    float coeffa[30];

    for (i=0; i<=degree; i++){
        coeffa[i]=coeff[i]; /* save input */
    }


    for (r=1; r<= degree; r++){
        for (i=0; i<= degree - r; i++){
            t1=(degree*t-i)/((float) r);
            coeffa[i]= (1.0-t1)* coeffa[i]  +   t1 * coeffa[i+1] ;
        }  
    }
        

    return (coeffa[0]);
}


// ::::::::::::::
// area.c
// ::::::::::::::
float area(p1,p2,p3)
/* find area of 2D triangle p1,p2,p3 */

	float p1[2], p2[2], p3[2];
{
	return( (p2[0]-p1[0])*(p3[1]-p1[1])-(p2[1]-p1[1])*(p3[0]-p1[0]))/2.0;
}


// ::::::::::::::
// bessel_ends.c
// ::::::::::::::
void bessel_ends(data,knot,l)
/*	Computes B-spline points data[1] and data[l+]
	according to Bessel end condition.

input:	data:	sequence of data coordinates data[0] to data[l+2].
		Note that data[1] and data[l+1] are expected to
		be empty, as they will be filled by this routine.
	knot:	knot sequence
	l:	number of intervals
 
output:	data:	completed, as above.
*/
	float data[], knot[];
	int l;
{
	float alpha, beta; int i;
	
	if (l==1)
	{/*  This is not really Bessel, but then what do you do
	     when you have only one interval? -- make it linear!
	 */
		data[1]= (2.0*data[0] + data[3])/3.0;
		data[2]= (2.0*data[3] + data[0])/3.0;
	}

	else if (l==2)
	{
		/* beginning:    */
		alpha= (knot[2]-knot[1])/(knot[2]-knot[0]);
		beta = 1.0 - alpha;

		data[1]=(data[2]-alpha*alpha*data[0]-beta*beta*data[4])
		/(2.0*alpha*beta);
		data[1]=2.0*(alpha*data[0]+beta*data[1])/3.0 + data[0]/3.0;

		/* end:  */
		alpha= (knot[2]-knot[1])/(knot[2]-knot[0]);
		beta = 1.0 - alpha;

		data[3]=(data[2]-alpha*alpha*data[0]-beta*beta*data[4])
		/(2.0*alpha*beta);

		data[3]=2.0*(alpha*data[3]+beta*data[4])/3.0+data[4]/3.0;	
	}
	
	else

	{


		/* beginning:    */
		alpha= (knot[2]-knot[1])/(knot[2]-knot[0]);
		beta = 1.0 - alpha;

		data[1]=(data[2]-alpha*alpha*data[0]-beta*beta*data[3])
		/(2.0*alpha*beta);
		data[1]=2.0*(alpha*data[0]+beta*data[1])/3.0 + data[0]/3.0;


		/* end:  */
		alpha= (knot[l]-knot[l-1])/(knot[l]-knot[l-2]);
		beta = 1.0 - alpha;

		data[l+1]=(data[l]-alpha*alpha*data[l-1]-beta*beta*data[l+2])
		/(2.0*alpha*beta);

		data[l+1]=2.0*(alpha*data[l+1]+beta*data[l+2])/3.0+data[l+2]/3.0;

	}

}

// ::::::::::::::
// bez_to_points.
// ::::::::::::::
void bez_to_points(degree,npoints,coeff,points)
/*	Converts Bezier curve into point sequence. Works on
	one coordinate only.
 
	Input:   degree:  degree of curve.
	         npoints: # of coordinates to be generated. (counting
		          from 0!)
	         coeff:   coordinates of control polygon.
	Output:  points:  coordinates of points on curve.

	Remark: For a 2D curve, this routine needs to be called twice,
		once for the x-coordinates and once for y.
*/

	int degree, npoints;
	float coeff[],points[];

{	
	float t,delt;
	int i;
	float hornbez();
	int k;

	delt=1.0/(float)npoints;
	t=0.0;
	for(i=0; i<=npoints; i++) 
	{
		points[i]=hornbez(degree,coeff,t);
		t = t+delt;
	}
}

// ::::::::::::::
// bez_to_power.
// ::::::::::::::

void bezier_to_power(degree,bez,coeff)
/*	Converts Bezier form to power (monomial) form. Works on
	one coordinate only.
 
	Input:   degree:  degree of curve.
	         bez:	  coefficients of Bezier form
	Output:  coeff:   coefficients of power form.

	Remark: For a 2D curve, this routine needs to be called twice,
		once for the x-coordinates and once for y.
*/

	int degree;
	float coeff[], bez[];

{	
	float  i_factorial, n_r;
	int i;

	differences(degree,bez,coeff); /* compute forward differences    */
				       /* and store them in coeff.       */

					/* Note that i_factorial is int. */
					/* For high degrees: danger!     */
	coeff[0]=bez[0];
	i_factorial=1;	
	n_r=1;
	for (i=1; i<=degree; i++)
	{
		i_factorial=i_factorial*i;
		n_r= n_r*(degree-i+1);

		coeff[i]=n_r*coeff[i]/i_factorial;
	}
}

// ::::::::::::::
// bspl_kappas.c
// ::::::::::::::

#include <stdio.h>
#include <math.h>
extern FILE *outfile;

void bspl_kappas(bspl_x,bspl_y,bspl_w,knot,l,dense)
/*	writes curvatures of cubic rational  B-spline curve into 
	a file.
input:
	bspl_x,bspl_y:  2D rat. B-spline polygon
	bspl_w:	the  B-spline weights
	knot:   the knot sequence
	dense:	how many curvature values to compute per interval
	l:      no. of intervals
output:
	written into file outfile
*/

	float bspl_x[],bspl_y[],bspl_w[],knot[];
	int  dense,l;
{
	float bez_x[300],bez_y[300], bez_w[300];
	float bleftx[4],blefty[4],brightx[4],brighty[4];
	float wleft[4], wright[4],coeffx[4],coeffy[4],weight[4];
	float dist,t,delt,h,u,diff;
	int i,j,i3;
	float curvature_0(),curvature_1(),abs();

	/* first, convert B-spline to Bezier: */

	ratbspline_to_bezier(bspl_x,bspl_y,bspl_w,knot,l,
	                     bez_x,bez_y,bez_w);

	/* Now plot kappas for each interval. */

	fprintf(outfile,"%d  %d\n",dense*l,dense);
	for(i=0; i<l; i++)
	{	i3=i*3;
		u= knot[i];  /*starting value for i-th segment*/
		diff=knot[i+1]-knot[i];
		for(j=0;j<=3;j++)
		{       /* create i-th cubic Bezier curve: */ 
			coeffx[j]=bez_x[i3+j]; 
			coeffy[j]=bez_y[i3+j];
			weight[j]=bez_w[i3+j];
		}


		delt=1.0/(float)dense;

		for(t=0.0; t<0.5; t=t+delt)
		{
		subdiv(3,coeffx,weight,t,bleftx,brightx,wleft,wright);
		subdiv(3,coeffy,weight,t,blefty,brighty,wleft,wright);
		h=curvature_0(brightx,brighty,wright,3);

		/* to print file, we'll have to use the GLOBAL
		   parameter u!   */

		u=knot[i]+t*diff;
		fprintf(outfile,"%2.3f  %f\n",u,h);
		}

		for(t=0.5; t<0.999; t=t+delt)
		{
		subdiv(3,coeffx,weight,t,bleftx,brightx,wleft,wright);
		subdiv(3,coeffy,weight,t,blefty,brighty,wleft,wright);

		h=curvature_0(bleftx,blefty,wleft,3);
		h=0.0-h;
		/* minus sign since order of polygon 
		   traversal is reversed!
		*/
		u=knot[i]+t*diff;
		fprintf(outfile,"%2.3f    %f\n",u,h);
		}
	}
	/* print very last kappa: */
	t=1.0;
	h=curvature_1(coeffx,coeffy,weight,3);
	fprintf(outfile,"%2.3f    %f\n",knot[l],h);

}

// ::::::::::::::
// c2_spline.c
// ::::::::::::::

void c2_spline(knot,l,data_x,data_y,bspl_x,bspl_y)
/* Finds the C2 cubic spline interpolant to
the data points in data_x, data_y.
Input:	knot:	the knot sequence  knot[0], ..., knot[l]
	l:	the number of intervals
	data_x, data_y: the data points data_x[0], ...,
		data[l+2]. Attention:
		data_x[1] and data_x[l+1] are filled
		by  Bessel end conditions and are
		thus ignored on input. Same for data_y.
Output:	bspl_x, bspl_y: the B-spline control polygon of
		the interpolant. Dimensions:
		bspl_x[0], ..., bspl_x[l+2]. Same for
		bspl_y.
*/
	float knot[],data_x[],data_y[],bspl_x[],bspl_y[];
	int l;
{
	float alpha[100], beta[100], gamma[100], up[100], low[100];
	set_up_system(knot,l,alpha,beta,gamma);

	l_u_system(alpha,beta,gamma,l,up,low);

	bessel_ends(data_x,knot,l);
	bessel_ends(data_y,knot,l);

	solve_system(up,low,gamma,l,data_x,bspl_x);
	solve_system(up,low,gamma,l,data_y,bspl_y);

}


// ::::::::::::::
// check_flat.c
// ::::::::::::::
#include <math.h>

int check_flat(bx,by,degree,tol)
/* Checks if a polygon is flat. If all points
   are closer  than tol to the connection of the 
   two endpoints, then it is flat. Crashes if the endpoints  
   are identical.

Input:	bx,by, degree: the Bezier curve
	tol:            tolerance

*/
	float bx[],by[];
	int degree; float tol;
{
	float dist;
	int i;
	float height();

	for (i=1; i<degree; i++)
	{
		dist = height(bx[i],by[i],bx[0],by[0],
			bx[degree],by[degree]);
		if (fabs(dist) > tol)  return(0);
	}
	
	return(1);
}

// ::::::::::::::
// conic_weight.c
// ::::::::::::::
#include <math.h>
float conic_weight(b0,b1,b2,p)
/* 
Input:	b0,b1,b2:	conic control polygon
	p:		point on conic
Output:	weight of b1 (assuming standard form).

For method, check Farin  14.5, 14.6.
*/

	float b0[],b1[],b2[],p[];
{
	float area(),length_2();
	float tau0,  tau2 , t,t1, d, dist, weight;
	float b11[2], b10[2];

	tau0=area(b1,p,b2);
	tau2=area(b1,b0,p);
	return(area(b0,b2,p)/(2.0*sqrt(tau0*tau2))); /* weight of b1         */

}

// ::::::::::::::
// curvature_0.c
// ::::::::::::::
#include <math.h>
float curvature_0(bez_x,bez_y,weight,degree)
/* computes curvature of rational  Bezier curve at t=0 */
	float bez_x[], bez_y[], weight[];
	int degree;
{
	float b0[2],b1[2],b2[2];
	float dist;
	float area();

	b0[0]=bez_x[0]; b1[0]=bez_x[1]; b2[0]=bez_x[2];
	b0[1]=bez_y[0]; b1[1]=bez_y[1]; b2[1]=bez_y[2];

	dist = sqrt( (b1[0]-b0[0])*(b1[0]-b0[0])+
	             (b1[1]-b0[1])*(b1[1]-b0[1])  );


	return (2.0*(degree-1)*weight[0]*weight[2]*area(b0,b1,b2)
	       /(degree*weight[1]*weight[1]*dist*dist*dist));
}

// ::::::::::::::
// deboor.c
// ::::::::::::::
float deboor(degree,coeff,knot,u,i)
/*
	uses de Boor algorithm to compute one
	coordinate on B-spline curve for param. value u in interval i.
input:  degree:	polynomial degree of each piece of curve
	coeff:	B-spline control points
	knot:	knot sequence
	u:	evaluation abscissa
	i:	u's interval: u[i]<= u < u[i+1]
output:	coordinate value. 
*/

	float coeff[],knot[];
	float u;
	int degree,i;
{
	int k,j;
	float t1,t2;
	float coeffa[30];  /* might need adjustment! */
	
	for (j=i-degree+1; j<=i+1; j++)coeffa[j]=coeff[j];

	for (k=1; k<= degree; k++)
	for ( j=i+1 ;j>=i-degree+k+1; j--)
	{
		t1= (knot[j+degree-k] - u )/(knot[j+degree-k]-knot[j-1]);
		t2= 1.0-t1;

		coeffa[j]=t1* coeffa[j-1]+t2* coeffa[j];
	}	
	return (coeffa[i+1]);
}


