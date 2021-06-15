#include <iostream>
#include <random>
#include <vector>
#include <fstream>

using namespace std;

// l → length of the box
void pbc(double *r, const double &l) {
  r[0] -= l*rint(r[0]/l); // assume square box
  r[1] -= l*rint(r[1]/l); // i.e. lx = ly = l
}

struct Particle {
  int pid;
  double r[2];
  double D;
  double qe; // The prob at which a particle 
              // shows fluorescence (quantum efficiency?)

  // debug stuffs
  long last_flash_tstep;              
};

int main(int argc, char** argv) {

  // Setup the simualtion box
  int N = 300.0;         // # of particles, fluorophores in our case
  double rho = 1.2;      // number density of particles
  double l = sqrt(N/rho);// length of the box, assume square domain

  cout << "# l = " << l << endl;

  // Time evolution parameters
  unsigned long tsteps_max = 100000; //10^5
  double dt = 1e-3;

  // Laser/APD target sampling volume - assume circular, radius r centered at origin
  double samp_r = 1.0; 
  double samp_rsq = samp_r*samp_r; 

  // config params
  int conf_save_interval = 1000; //10^3

  // Initialize PRNG
  random_device rd;
  mt19937 mt(rd());
  uniform_real_distribution<double> u_dist(0.0, 1.0); // Uniform dist
  normal_distribution<double> gauss_dist(0.0, 1.0); // Normal dist. for Langevin 

  // setup particle array
  vector<Particle> particles;
  particles.resize(N);  //reserve and construct

  //Particle Init
  for(int i=0; i<N; i++) {
    particles[i].pid = i;
    particles[i].r[0] = 0.5*l*2*(u_dist(mt)-0.5); // Put particles at random positions 
    particles[i].r[1] = 0.5*l*2*(u_dist(mt)-0.5); // in thew interval [-l/2, l/2]
    particles[i].D = 3.0;  // Diffusion const. of i-th particle 
                            // Fluctuation-Dissipassion says d = gamma/(k_B*T)
    particles[i].qe = 0.8; // const. for the time being

    particles[i].last_flash_tstep = -1;
  }

  // Save initial config
  vector<Particle> particles_init = particles;

  // files
  ofstream fconf, fthermo;
  fconf.open("config.txt", ios::out);
  fthermo.open("thermo.txt", ios::out);

  // compute variables
  double msd = 0.0;

  // Now time evolve → Simulation Loop
  for(unsigned long it=0; it<tsteps_max; it++) {

    for(auto &i : particles) {
      i.r[0] += sqrt(4*i.D*dt)*gauss_dist(mt); //sqrt(2*dim*D*time_step)*Gauss_Displacement(RND)
      i.r[1] += sqrt(4*i.D*dt)*gauss_dist(mt);

      // Apply periodic boundary condition
      pbc(i.r, l);
    }

    // Do I flash?
    double count=0.0;
    for(int i=0; i<N; i++) {
      // am i in APD's eye?
      if( (particles[i].r[0]*particles[i].r[0] + particles[i].r[1]*particles[i].r[1]) <= samp_rsq ) {
          
          //if( it==0 || (it-particles[i].last_flash_tstep)>=0 ) {
            
            // Try flashing with qe probability
            if( u_dist(mt) < particles[i].qe ) {
              particles[i].last_flash_tstep = it;
              count++;
            }
          //}
      }
    }

    // Calculate MSD
    double msd_it = 0.0;
    for(int i=0; i<N; i++) {
      double dr_[2] ;
      dr_[0] = (particles[i].r[0]-particles_init[i].r[0]);
      dr_[1] = (particles[i].r[1]-particles_init[i].r[1]);
      pbc(dr_, l); //Is this needed?
      msd_it += (dr_[0]*dr_[0] + dr_[1]*dr_[1]);
    }

    msd += (msd_it)/double(N);
    fthermo << it*dt << " " << msd/double(it+1) << " " << count << endl;
          //Time elapsed                     //No of "active flourophores"
    
    // save config
    if(it % conf_save_interval == 0) {
      for(auto &i : particles) {
          fconf << i.r[0] << " " << i.r[1] << " " 
                << (i.last_flash_tstep == it) << endl;
      } 
      fconf << endl << endl;  
    }

  }

  // Cleanup
  fconf.close();
  fthermo.close();


  return 0;
}