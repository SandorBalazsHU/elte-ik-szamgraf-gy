#version 430

// pipeline-ból bejövő per-fragment attribútumok
in vec4 vs_out_color;
in vec2 vs_out_normPos;

// kimenő érték - a fragment színe
out vec4 fs_out_col;

uniform float ElapsedTimeInSec = 0.0;

uniform float shiftx    = 0.5f;
uniform float shifty    = 0.7f;
uniform float zoom      = 5.0f;
uniform int   iteration = 35;

// 1. Feladat
void main_elso()
{
	fs_out_col = vs_out_color;
	/*fs_out_col = vec4( // A vec4 4 db floatot vár.
		vec3( 1.0 ) /*A "broadcast" konstruktor 1 db. floatból csinál 3db-ot. - vs_out_color.rgb, // A 4-ből hármat itt megadunk. 
		// A vec3-ra értelmezettek a műveletek, ezek koordinátánként hajtódnak végre....
		1.0 ); // ... majd az alfát 1 re állítjuk, ez a 4. float.*/
}

///

// 2. Feladat
void main_masodik()
{

	fs_out_col = vec4( vs_out_normPos, 0.0, 1.0 );
}

///
// 3. Feladat
const float PI = 3.14159265359;

float myFunc( const float t )
{
	//return abs( cos ( t * 2.0 * PI + ElapsedTimeInSec ) ); 
	return pow(abs(cos( t * 2.0 * PI + ElapsedTimeInSec)),8.0);// Figyelem a konstansokra!
}

void main_harmadik()
{
	fs_out_col = vec4(					// A vec4 4 db floatot vár.
	vec3( myFunc( vs_out_normPos.x ) ), // A myFun 1 db floattal tér vissza, a broadcast konstruktor 1 db. floatból csinál 3db-ot
	1.0 );								// az alfát 1 re állítjuk, ez a 4. float.
}

///

// 4. Feladat

const float angle_degree = 30.0; // konstans változó, ahogy a feladatban szerepelt.

void main_negyedik()
{
	float angle_rad = radians( angle_degree ); // fokoról radiánra, beépített változó
	vec2 dir = vec2( cos( angle_rad ), sin( angle_rad ) ); // Radiánból az irány vektorra, ez vec2 lesz.

	fs_out_col = vec4( 
		vec3( 
		myFunc( 
		dot( dir, vs_out_normPos ) // megnézzük a pixel normalizált koordinátájának az adott irány menti vetületét. dot beépített függvény.
		)						   // A myFun 1 db floattal tér vissza, a broadcast konstruktor 1 db. floatból csinál 3db-ot
		), 1.0 );                  // az alfát 1 re állítjuk, ez a 4. float.
}

// 5. Feladat

vec4 colorize( float between0and1 )
{
	const vec4 colorLower = vec4( 0.0,0.0,0.0,1.0);//vec4( 0.94, 0.23, 0.125, 1.0 ); // Első szín
	const vec4 colorHigher = vec4(1.0,2.0,3.0,1.0);//vec4( 0.1, 0.56, 0.6, 1.0 );   // Második szín

	return mix( colorLower, colorHigher, between0and1 ); // a mix beépített függvény az első két paramétert (ami lehet float, vec2, vec3, vec4 is) 
														 // lineárisan interpolálja a harmadik paraméter szerint. 
														 // Tehát mix(a,b,t) = ( 1.0 - t ) * a + t * b. Persze ez be van "huzalozva" a GPU-kba.
}

void main_otodik()
{
	float angle_rad = radians( angle_degree );
	vec2 dir = vec2( cos( angle_rad ), sin( angle_rad ) );

	fs_out_col = colorize( myFunc( dot( dir, vs_out_normPos ) ) ); // Meghívjuk a coloriye fv.t. A MyFunc úgy lett kitalálva, hogy 0 és 1 közötti értéket adjon vissza.
}

// 6. Feladat

const vec4 angles_degree = vec4(30.0,140.0,236.0,342.0);

void main_hatodik()
{
	float func = 0.0;
	for ( int i = 0; i < 4; ++i)
	{
		float angle_rad = radians( angles_degree[i] );
		vec2 dir = vec2( cos( angle_rad ), sin( angle_rad ) );
		func +=  myFunc( dot( dir, vs_out_normPos ) );
	}
	func /= 4.0;
	fs_out_col = colorize( clamp( func, 0.0, 1.0 ) );
}

void main_gyakorlo()
{
	float r = length( vs_out_normPos );
	float phi = atan( vs_out_normPos.y, vs_out_normPos.x );
	fs_out_col = colorize( myFunc( r + phi / ( 2.0 * PI ) ) );
}

float mandelbrot_element(vec2 inputVector, int n){
    vec2 z = vec2(0.0, 0.0);
    for (int i = 0; i < n; ++i)
    {
		z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + inputVector;
    }
	//return length(z)/2.0f;
	//Ha ez a tag komplex értelemben kisebb mint 2.0, akkor térjen vissza 1.0. Különben 0.0-val.
	if (length(z) < 2.0)
    {
        return length(z)/2;
    }
	else
	{
		return 0.0f;
	}
}

void mandelbrot(){
	//Skálázza át a bejövő vs_out_normPos –t a [-2,-1.12]-[0.47,1.12] tartományba!
    vec2 scaled_vector;
	//Megjegyzés: A koordinátát beskálázom 0-1 közé a helyes méretezéshez.
    scaled_vector.x = mix(-2.0f, 0.47f, (vs_out_normPos.x+1.0f)/zoom)+shiftx;
    scaled_vector.y = mix(-1.12f, 1.12f, (vs_out_normPos.y+1.0f)/zoom)+shifty;
	float mandelbrot = mandelbrot_element(scaled_vector, iteration);
	fs_out_col = colorize( mandelbrot );
}

void main()
{
	// main_elso();
	// main_masodik();
	// main_harmadik();
	// main_negyedik();
	// main_otodik();
	// main_hatodik();
	// main_gyakorlo();
	mandelbrot();

}
