#include <stdio.h>
#include <math.h>


// defines for taxi_approx3
#define TAN_PI_6  0.577350269189626
#define TAN_PI_12 0.267949192431123

// defines for taxi_approx2
#define TAN_PI_8  0.414213562373095

#define N 200
#define X 0
#define Y 1

#define SQUARE(x) ((x)*(x))
#define REF(g, i, j) g[(i)*N + (j)]
#define MIN(a,b) ((a) < (b) ? (a) : (b))

// Error without delta: 0.0000188250658
// Error with delta:    0.0000075845663
// That is 7.6e-6
double taxi_approx64_delta(double x, double y){
  static const double tans[65] = {0.0, 0.01227246237956628, 0.02454862210892544, 0.03683218099484564, 0.04912684976946725, 0.06143635258159376, 0.07376443152244928, 0.08611485119762791, 0.09849140335716425, 0.110897911595913, 0.1233382361367387, 0.1358162787093877, 0.1483359875383474, 0.1609013624534892, 0.1735164601378558, 0.1861853995275837, 0.198912367379658, 0.2117016240239833, 0.2245575093171293, 0.2374844488160702, 0.2504869601913055, 0.263569659899918, 0.2767372701404143, 0.2899946261126061, 0.3033466836073424, 0.3167985269526038, 0.330355377344334, 0.3440226015924263, 0.3578057213145241, 0.3717104226127435, 0.3857425662711212, 0.3999081985145372, 0.414213562373095, 0.4286651096994995, 0.4432695138908643, 0.4580336833706724, 0.4729647758913199, 0.4880702137228629, 0.5033576997992942, 0.5188352348999757, 0.5345111359507916, 0.5503940555372639, 0.566493002730344, 0.5828173653349761, 0.5993769336819237, 0.616181926094866, 0.6332430161775691, 0.6505713620801533, 0.6681786379192989, 0.6860770675448629, 0.7042794608650442, 0.7227992529642059, 0.7416505462720354, 0.7608481560702512, 0.7804076596539435, 0.8003454494993202, 0.8206787908286602, 0.8414258840072547, 0.8626059322567399, 0.8842392152253498, 0.906347169019147, 0.9289524733703675, 0.9520791467009252, 0.9757526499323765, 1.0};
  static const double cache[65] = {1.0, 1.000075303831095, 1.000301272041302, 1.000678074885643, 1.001205996470393, 1.001885435276175, 1.002716904892817, 1.003701034968476, 1.004838572376311, 1.006130382602739, 1.007577451362089, 1.009180886443291, 1.010941919795087, 1.012861909857108, 1.014942344145111, 1.017184842099628, 1.019591158208318, 1.022163185413362, 1.024902958816449, 1.027812659695079, 1.030894619845249, 1.034151326266982, 1.037585426210666, 1.041199732603783, 1.044997229879378, 1.048981080229448, 1.05315463030854, 1.057521418414975, 1.062085182179568, 1.066849866794267, 1.071819633815984, 1.076998870583968, 1.082392200292394, 1.088004492763556, 1.093840875971018, 1.099906748366472, 1.106207792068889, 1.112749986979771, 1.11953962589416, 1.126583330683404, 1.133888069632715, 1.141461176024291, 1.14931036806532, 1.157443770269642, 1.165869936412268, 1.174597874187579, 1.183637071714833, 1.192997526048913, 1.202689773870091, 1.212724924544292, 1.22311469576502, 1.233871452010141, 1.24500824607133, 1.256538863941542, 1.268477873376807, 1.280840676483328, 1.29364356671998, 1.306903790750257, 1.320639615627412, 1.334870401852683, 1.349616682910011, 1.364900251952839, 1.380744256400425, 1.397173301294458, 1.414213562373095};
  static const double divs[64] = {81.48324020654617, 81.45869897802326, 81.40963130369587, 81.33606674009614, 81.23804959976664, 81.11563892456739, 80.96890845011201, 80.79794656135175, 80.60285623933564, 80.38375499917866, 80.14077481927563, 79.87406206180044, 79.58377738454598, 79.27009564414729, 78.93320579075522, 78.57331075421982, 78.19062732185495, 77.78538600784995, 77.35783091441968, 76.90821958476502, 76.43682284793906, 75.94392465570959, 75.42982191151566, 74.89482429162886, 74.33925405860933, 73.76344586719139, 73.16774656269929, 72.55251497211843, 71.9181216879546, 71.26494884500138, 70.59338989015386, 69.90384934541692, 69.19674256423025, 68.47249548127898, 67.731544355924, 66.97433550941749, 66.20132505605467, 65.41297862842823, 64.60977109694736, 63.79218628379614, 62.96071667149342, 62.11586310624337, 61.25813449624342, 60.38804750513489, 59.50612624078624, 58.6129019395908, 57.70891264646706, 56.79470289076401, 55.87082335825257, 54.93783055941768, 53.99628649423485, 53.04675831364147, 52.08981797790978, 51.12604191211419, 50.15601065891911, 49.18030852888142, 48.19952324847784, 47.21424560608863, 46.22506909612424, 45.2325895615254, 44.2374048348529, 43.24011437817475, 42.241318921969, 41.24162010327328};
  double tmp;
  /* Make x > y true */
  if(y > x){
    tmp = x;
    x = y;
    y = tmp;
  }
  /* Binary search. Should maybe be unrolled explicitly in Arduino...
   * Average number of binary-search-iterations is 6.0 (on the square testing grid)
   * Average number of linear-search-iterations is 36.0 (also on the square testing grid)
   * While form of binary search uses ca 8 operations per iteration?
   * Unrollong loop saves ca four operations per iteration?
   * (two assignments, one jmp and on stop criterion comparison)? */
  int i   = 32;
  int first = 0;
  int last  = 64;

  do{
    if (y <= x*tans[i]){
      last = i;
    }else{
      first = i;
    }
    i = (first + last) >> 1;
  } while(last - first != 1);

  /* With this number, it's possible to make the optimal linear combination
   * of two cached values to get as close to sqrt(x*x + y*y) as possible */
  double c  = divs[i]*(tans[i]*x - y);

  /** The linear compensation of the error **
   ** Don't really know if this is economical use of clock cycles 
   ** compared to just adding more cached tans **/

  /* These variables could probably be saved in on to save clock cycles */
  double delta_from_high = (tans[i+1]*x - y);
  double delta_from_low  = (y - tans[i]*x);
  double lin_delta;
  /* These two ifs could be combined to sometimes save an assignment.
   * Would look less clear though. */
  if(delta_from_high < delta_from_low){
    lin_delta = delta_from_high;
  }else{
    lin_delta = delta_from_low;
  }
  /* Giving the compensation a flat top to closer resemble
   * the curve that we're really trying to compensate for */
  if(lin_delta > 0.3*x*(tans[i+1] - tans[i])){
    lin_delta = 0.3*x*(tans[i+1] - tans[i]);
  }

  // 0.0030531 found by trial and error (largest value with no negative error)
  return(((x + c)*cache[i] - c*cache[i+1] - 0.0030531*lin_delta));
}

// Error without delta: 0.0000753037392
// Error with delta:    0.0000301140586
double taxi_approx32_delta(double x, double y){
  static const double tans[33] = { 0.000000000000000,0.024548622108925,0.049126849769467,0.073764431522449,0.098491403357164,0.123338236136739,0.148335987538347,0.173516460137856,0.198912367379658,0.224557509317129,0.250486960191305,0.276737270140414,0.303346683607342,0.330355377344334,0.357805721314524,0.385742566271121,0.414213562373095,0.443269513890864,0.472964775891320,0.503357699799294,0.534511135950792,0.566493002730344,0.599376933681924,0.633243016177569,0.668178637919299,0.704279460865044,0.741650546272035,0.780407659653944,0.820678790828660,0.862605932256740,0.906347169019147,0.952079146700925,1.000000000000000};
  static const double cache[33] = {1.00000000000000,1.00030127204130,1.00120599647039,1.00271690489282,1.00483857237631,1.00757745136209,1.01094191979509,1.01494234414511,1.01959115820832,1.02490295881645,1.03089461984525,1.03758542621067,1.04499722987938,1.05315463030854,1.06208518217957,1.07181963381598,1.08239220029239,1.09384087597102,1.10620779206889,1.11953962589416,1.13388806963272,1.14931036806532,1.16586993641227,1.18363707171483,1.20268977387009,1.22311469576502,1.24500824607133,1.26847787337681,1.29364356671998,1.32063961562741,1.34961668291001,1.38074425640042,1.41421356237309};
  static const double divs[33] = {40.7354838720833,40.6864161977559,40.5883990574263,40.4416685829710,40.2465782609548,40.0035980810516,39.7133134037971,39.3764235504049,38.9937401180398,38.5661850246095,38.0947882877836,37.5806855435902,37.0251153105707,36.4294160060783,35.7950227219147,35.1234637670676,34.4163569858810,33.6754058605261,32.9023954071634,32.0991878756827,31.2677182633801,30.4099896533800,29.5280683890315,28.6240790959081,27.7001995633968,26.7586554982137,25.8017151624817,24.8316839092870,23.8508986288838,22.8617221189191,21.8665373922468,20.8677419360416};
  int i = 1;
  double tmp;
  /* Make x > y true */
  if(y > x){
    tmp = x;
    x = y;
    y = tmp;
  }
  /* This search is linear, could be made binary */
  for(; i < 33; i++){
    if(y <= tans[i]*x){
      break;
    }
  }

  double delta_from_high = (tans[i]*x - y);
  double delta_from_low  = (y - tans[i-1]*x);
  double lin_delta;
  /* These two ifs could be combined to sometimes save an assignment.
   * Would look less clear though. */
  if(delta_from_high < delta_from_low){
    lin_delta = delta_from_high;
  }else{
    lin_delta = delta_from_low;
  }
  if(lin_delta > 0.3*x*(tans[i] - tans[i-1])){
    lin_delta = 0.3*x*(tans[i] - tans[i-1]);
  }

  double c  = divs[i-1]*(tans[i-1]*x - y);
  /* The value 0.0000301145121 is the lowest error I managed to get
   * while keeping all error still positive (error grows faster after it gets a negative part).
   * I tuned that with the value 0.00613 */
  //return((1.0 - 0.5*0.0000301145121)*((x + c)*cache[i-1] - c*cache[i] - 0.00613*lin_delta));
  return(((x + c)*cache[i-1] - c*cache[i] - 0.00613*lin_delta));
}

double delta_pt(double x, double y){
  static const double tans[3]  = { 0.000000000000000, TAN_PI_8, 1.0 };
  double tmp;
  if(y > x){
    tmp = x;
    x = y;
    y = tmp;
  }

  int i = 1;
  for(; i < 3; i++){
    if(y <= tans[i]*x){
      break;
    }
  }

  double delta_from_high;
  double delta_from_low;
  double lin_delta;
  /* The factors 0.7 and 1.3 shifts triangle roofs away from pi/4-line.
   * Not useful, but a bit funny. */
  delta_from_high = (tans[i]*x - y)  *0.7;
  delta_from_low  = (y - tans[i-1]*x)*1.3;

  /* A min function using if.
   * Makes a triangular shape with as many
   * linearly growing arms like r=sqrt(x*x+y*y
   * Angles covered by tans have zero delta, and
   * triangularly shaped delta values between them. */
  if(delta_from_high < delta_from_low)
    lin_delta = delta_from_high;
  else
    lin_delta = delta_from_low;

  /* Give triangles flat top to more closely approximate sine-form
     This takes error down from 0.0096 to 0.0069
     The number 0.3 is just guessed with a little trial and error */
  if(lin_delta > 0.3*x*(tans[i] - tans[i-1]))
    lin_delta = 0.3*x*(tans[i] - tans[i-1]);

  // The value 0.1323 just worked well with the three lines, taxi_approx3 and the 0.3 roof...
  return(0.1323*lin_delta);
}

// Error amplitide: 0.00753 %
// Max error: 0.00376491230 %
double taxi_approx32(double x, double y){
  static const double tans[33] = { 0.000000000000000,0.024548622108925,0.049126849769467,0.073764431522449,0.098491403357164,0.123338236136739,0.148335987538347,0.173516460137856,0.198912367379658,0.224557509317129,0.250486960191305,0.276737270140414,0.303346683607342,0.330355377344334,0.357805721314524,0.385742566271121,0.414213562373095,0.443269513890864,0.472964775891320,0.503357699799294,0.534511135950792,0.566493002730344,0.599376933681924,0.633243016177569,0.668178637919299,0.704279460865044,0.741650546272035,0.780407659653944,0.820678790828660,0.862605932256740,0.906347169019147,0.952079146700925,1.000000000000000};
  static const double cache[33] = {1.00000000000000,1.00030127204130,1.00120599647039,1.00271690489282,1.00483857237631,1.00757745136209,1.01094191979509,1.01494234414511,1.01959115820832,1.02490295881645,1.03089461984525,1.03758542621067,1.04499722987938,1.05315463030854,1.06208518217957,1.07181963381598,1.08239220029239,1.09384087597102,1.10620779206889,1.11953962589416,1.13388806963272,1.14931036806532,1.16586993641227,1.18363707171483,1.20268977387009,1.22311469576502,1.24500824607133,1.26847787337681,1.29364356671998,1.32063961562741,1.34961668291001,1.38074425640042,1.41421356237309};
  static const double divs[33] = {40.7354838720833,40.6864161977559,40.5883990574263,40.4416685829710,40.2465782609548,40.0035980810516,39.7133134037971,39.3764235504049,38.9937401180398,38.5661850246095,38.0947882877836,37.5806855435902,37.0251153105707,36.4294160060783,35.7950227219147,35.1234637670676,34.4163569858810,33.6754058605261,32.9023954071634,32.0991878756827,31.2677182633801,30.4099896533800,29.5280683890315,28.6240790959081,27.7001995633968,26.7586554982137,25.8017151624817,24.8316839092870,23.8508986288838,22.8617221189191,21.8665373922468,20.8677419360416};
  int i = 1;
  double tmp;
  double c;
  /* Make x > y true */
  if(y > x){
    tmp = x;
    x = y;
    y = tmp;
  }
  /* This search is linear, could be made binary */
  for(; i < 33; i++){
    if(y <= tans[i]*x){
      break;
    }
  }
  c  = divs[i-1]*(tans[i-1]*x - y);
  return 0.999962348130400*((x + c)*cache[i-1] - c*cache[i]);
}

// Error amplitude: 0.03 %
// Max error: 0.015 %
double taxi_approx16(double x, double y){
  static const double tans[17] = {0.000000000000000, // tan(0)
                                  0.049126849769467, // tan(   pi/(16*4))
                                  0.098491403357164, // tan(2 *pi/(16*4))
                                  0.148335987538347, // tan(3 *pi/(16*4))
                                  0.198912367379658, // tan(4 *pi/(16*4))
                                  0.250486960191305, // tan(5 *pi/(16*4))
                                  0.303346683607342, // tan(6 *pi/(16*4))
                                  0.357805721314524, // tan(7 *pi/(16*4))
                                  0.414213562373095, // tan(8 *pi/(16*4))
                                  0.472964775891320, // tan(9 *pi/(16*4))
                                  0.534511135950792, // tan(10*pi/(16*4))
                                  0.599376933681924, // tan(11*pi/(16*4))
                                  0.668178637919299, // tan(12*pi/(16*4))
                                  0.741650546272035, // tan(13*pi/(16*4))
                                  0.820678790828660, // tan(14*pi/(16*4))
                                  0.906347169019147, // tan(15*pi/(16*4))
                                  1.000000000000000};// tan(   pi/4)

  static const double cache[17] = {1.0,
                                   1.00120599647039,  //sqrt(1 + tans[ 1]*tans[ 1])
                                   1.00483857237631,  //sqrt(1 + tans[ 2]*tans[ 2])
                                   1.01094191979509,  //sqrt(1 + tans[ 3]*tans[ 3])
                                   1.01959115820832,  //sqrt(1 + tans[ 4]*tans[ 4])
                                   1.03089461984525,  //sqrt(1 + tans[ 5]*tans[ 5])
                                   1.04499722987938,  //sqrt(1 + tans[ 6]*tans[ 6])
                                   1.06208518217957,  //sqrt(1 + tans[ 7]*tans[ 7])
                                   1.08239220029239,  //sqrt(1 + tans[ 8]*tans[ 8])
                                   1.10620779206889,  //sqrt(1 + tans[ 9]*tans[ 9])
                                   1.13388806963272,  //sqrt(1 + tans[10]*tans[10])
                                   1.16586993641227,  //sqrt(1 + tans[11]*tans[11])
                                   1.20268977387009,  //sqrt(1 + tans[12]*tans[12])
                                   1.24500824607133,  //sqrt(1 + tans[13]*tans[13])
                                   1.29364356671998,  //sqrt(1 + tans[14]*tans[14])
                                   1.34961668291001,  //sqrt(1 + tans[15]*tans[15])
                                   1.41421356237309}; // sqrt(2)

  static const double divs[16] = {20.3554676249872, // 1/(tans[ 1] - tans[ 0])
                                  20.2574504846576, // 1/(tans[ 2] - tans[ 1])
                                  20.0623601626415, // 1/(tans[ 3] - tans[ 2])
                                  19.7720754853870, // 1/(tans[ 4] - tans[ 3])
                                  19.3893920530220, // 1/(tans[ 5] - tans[ 4])
                                  18.9179953161959, // 1/(tans[ 6] - tans[ 5])
                                  18.3624250831764, // 1/(tans[ 7] - tans[ 6])
                                  17.7280317990127, // 1/(tans[ 8] - tans[ 7])
                                  17.0209250178262, // 1/(tans[ 9] - tans[ 8])
                                  16.2479145644634, // 1/(tans[10] - tans[ 9])
                                  15.4164449521609, // 1/(tans[11] - tans[10])
                                  14.5345236878125, // 1/(tans[12] - tans[11])
                                  13.6106441553012, // 1/(tans[13] - tans[12])
                                  12.6537038195690, // 1/(tans[14] - tans[13])
                                  11.6729185391658, // 1/(tans[15] - tans[14])
                                  10.6777338124936};
  int i = 1;
  double tmp;
  double c;
  /* Make x > y true */
  if(y > x){
    tmp = x;
    x = y;
    y = tmp;
  }
  /* This search is linear, could be made binary */
  for(; i < 17; i++){
    if(y <= tans[i]*x){
      break;
    }
  }
  c  = divs[i-1]*(tans[i-1]*x - y);
  return 0.99985*((x + c)*cache[i-1] - c*cache[i]);
}

// Error: 0.86 %
double taxi_approx3(double x, double y){
  /* Make x > y true */
  double tmp;
  if(y > x){
    tmp = x;
    x = y;
    y = tmp;
  }

  /* The cached sqrt-values for four different angles:
   * {0, pi/12, pi/6, pi/4} */
  double cache[4] = {
                     1.0,
                     sqrt(1 + TAN_PI_12*TAN_PI_12),
                     sqrt(1 + TAN_PI_6*TAN_PI_6),
                     sqrt(2)
                    };

  /* Index of the lower angle line and higher angle line (index zero is the line along x-axis). */
  int l0, l1;

  /* Factors that will linearly combine sqrts.
   * Only two will be nonzero. */
  double f[4] = { 0.0 };

  /* Linear factor that makes f[l] + f[h] = x true */
  double c;

  /* Cached tangens of lower angle and higher angle */
  double th, tl;

  /* Result */
  double res = 0.0;
  
  if(y < TAN_PI_12*x){
    // when y = 0           we should have f0 = x, f1 = 0
    // when y = TAN_PI_12*x we should have f0 = 0, f1 = x
    l0 = 0;
    l1 = 1;
    tl = 0.0;
    th = TAN_PI_12;
  }else if(y < TAN_PI_6*x){
    // when y = TAN_PI_12*x we should have f1 = x, f2 = 0
    // when y = TAN_PI_6*x  we should have f1 = 0, f2 = x
    l0 = 1;
    l1 = 2;
    tl = TAN_PI_12;
    th = TAN_PI_6;
  }else{
    // when y = TAN_PI_6*x we should have f2 = x, f3 = 0
    // when y = x          we should have f2 = 0, f3 = x
    l0 = 2;
    l1 = 3;
    tl = TAN_PI_6;
    th = 1.0;
  }
  c  = (1.0/(th-tl))*(tl*x - y);
  f[l0] = x + c;
  f[l1] = -c;
  for(int i = 0; i < 4; i++){
    res += f[i]*cache[i];
  }

  return res;
}



// Error: 1.96 %
double taxi_approx2(double x, double y){
  double tmp, f0, f1, f2;

  if(y > x){
    tmp = x;
    x = y;
    y = tmp;
  }
  if(y < TAN_PI_8*x){
    // when y = 0    we should have f0 = x
    // when y = TAN_PI_8*x we should have f1 = x
    f0 = x - (1.0/TAN_PI_8)*y;
    f1 = (1.0/TAN_PI_8)*y;
    f2 = 0;
  }else{
    // when y = TAN_PI_8*x we should have f1 = x, f2 = 0
    // when x = y we should have    f1 = 0, f2 = x
    f0 = 0;
    f1 =     (1.0/(1-TAN_PI_8))*(x - y);
    f2 = x - (1.0/(1-TAN_PI_8))*(x - y);
  }

  return f0 + f1*sqrt(1 + TAN_PI_8*TAN_PI_8) + f2*sqrt(2);
}

// Two segments, but not equally large
double taxi_approx2_weird(double x, double y){
  float tmp, f0, f1, f2;
  if(y > x){
    tmp = x;
    x = y;
    y = tmp;
  }
  if(y < 0.5*x){
    // when y = 0    we should have f0 = x
    // when y = 0.5x we should have f1 = x
    f0 = x - 2*y;
    f1 = 2*y;
    f2 = 0;
  }else{
    // when y = 0.5x we should have f1 = x, f2 = 0
    // when x = y we should have    f1 = 0, f2 = x
    f0 = 0;
    f1 = 2*(x - y);
    f2 = x - 2*(x - y);
  }

  return f0 + f1*sqrt(1.25) + f2*sqrt(2);
}

// Error: 8.23 %
double taxi_approx1(double x, double y){
  float tmp, f0, f1;
  /* Make x > y true */
  if(y > x){
    tmp = x;
    x = y;
    y = tmp;
  }

  f0 = x - y;
  f1 = y;

  return f0 + f1*sqrt(2);
}

void taxi_norm(double l[N*N], double g[N*N][2]){
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      REF(l, i, j) = taxi_approx64_delta(REF(g,i,j)[X], REF(g,i,j)[Y]);
    }
  }
}

void calculate_deltas(double d[N*N], double g[N*N][2]){
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      REF(d, i, j) = delta_pt(REF(g,i,j)[X], REF(g,i,j)[Y]);
    }
  }
}

int init_pts_grid(double pts_grid[N*N][2]){
  double inc = (1.0/((double)N - 1.0));
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      REF(pts_grid, i, j)[X] = inc*(double)j;
      REF(pts_grid, i, j)[Y] = inc*(double)i;
    }
  }
  /* Results in origo are not interesing */
  REF(pts_grid, 0, 0)[X] = 0.0001;
  REF(pts_grid, 0, 0)[Y] = 0.0001;
  return 0;
}

void fprint_grid(FILE* f, char* ctrl_str, double l[N*N]){
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      fprintf(f, ctrl_str,  REF(l, i, j));
    }
    fprintf(f, "\n");
  }
}

void print_grid(double l[N*N]){
  fprint_grid(stdout, "% .3f ", l);
}

double grid_max(double g[N*N]){
  double max = 0.0;
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      if(REF(g, i, j) > max) max = REF(g, i, j);
    }
  }
  return max;
}

double grid_min(double g[N*N]){
  double min = 0.0;
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      if(REF(g, i, j) < min) min = REF(g, i, j);
    }
  }
  return min;
}

void print_pts_grid(double pts_grid[N*N][2]){
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      printf("(%.3f, %.3f) ",  REF(pts_grid,i, j)[X], REF(pts_grid,i, j)[Y]);
    }
    printf("\n");
  }
}

void l2_norm(double l[N*N], double pts_grid[N*N][2]){
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
    REF(l, i, j) = sqrt(SQUARE(REF(pts_grid, i, j)[X]) + SQUARE(REF(pts_grid, i, j)[Y]));
    }
  }
}

void residuals(double r[N*N], double a[N*N], double b[N*N]){
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      REF(r, i, j) =   REF(a, i, j) - REF(b, i, j);
    }
  }
}

void rel_err(double pr[N*N], double r[N*N], double l[N*N]){
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      REF(pr, i, j) = REF(r, i, j)/REF(l, i, j);
    }
  }
}

void save_grid(double g[N*N], char* filename){
  FILE* f;
  f = fopen(filename, "w");
  fprint_grid(f, "% 8.13f ", g);
  fclose(f);
  printf("info: saved a grid in file %s\n", filename);
}

void plot_grid(double g[N*N]){
  FILE * gnuplot_pipe;

  /* The Gnuplot script goes here */
  char* command = 
    /** The 3D-plot version using pm3d: **/
    "set term wxt 0 size 1200, 900 raise; "
    // "set pm3d map; " /* If a 3D surface is too slow... */
    "splot '-' matrix with pm3d notitle\n";

    /** For saving output as svg files: **/
    //"set term svg background 'white'; "
    //"set output '/tmp/b.svg'; "
    //"set contour base; " /* Draw contour lines. Could use a nicer color palette... */
    //"unset surface; "
    //"unset tics; "
    //"set view map; "
    //"splot '-' matrix with lines lw 0.3 notitle\n";

  printf("info: invoking gnuplot\n");
  fflush(stdout);

  /* Open the gnuplot pipe */
  gnuplot_pipe = popen("gnuplot -persistent", "w");

  /*Send commands to gnuplot one by one.*/
  fprintf(gnuplot_pipe, "%s \n", command);

  /* Send grid data to gnuplot */
  fprint_grid(gnuplot_pipe, "% 8.13f ", g);
  fprintf(gnuplot_pipe, "e\n");
  fflush(gnuplot_pipe);
  fclose(gnuplot_pipe);
}

int main(int argc, char** argv){
  double  pts_grid[N*N][2];
  double         l[N*N];
  double    taxi_l[N*N];
  //double  compensl[N*N];
  //double  compensr[N*N];
  //double compenspr[N*N];
  double         r[N*N];
  double        pr[N*N];
  //double    deltas[N*N];

  /* Initialize the grid of points */
  init_pts_grid(pts_grid);

  /* Find the correct L2-norms */
  l2_norm(l, pts_grid);

  /* Calculate the approximated L2-norms */
  taxi_norm(taxi_l, pts_grid);

  /* Calculate the compensating deltas.
   * Should have similar shape as r */
  //calculate_deltas(deltas, pts_grid);

  /* Try to compensate for error */
  //for(int i = 0; i < N; i++){
  //  for(int j = 0; j < N; j++){
  //    // Best match without roofs
  //    //REF(deltas, i, j)   *= 0.10065;
  //    // Best match with roofs
  //    REF(deltas, i, j)   *= 0.1323;
  //    REF(compensl, i, j)  = REF(taxi_l, i, j);
  //    REF(compensl, i, j) -= REF(deltas, i, j);
  //  }
  //}
  //residuals(compensr, compensl, l);

  /* Calculate the absolute errors in the approxiamtions */
  residuals(r, taxi_l, l);

  //plot_grid(r);
  //plot_grid(compensr);
  //plot_grid(deltas);

  /* Calculate relative errors in approximations */
  rel_err(pr, r, l);
  //rel_err(compenspr, compensr, l);

  /* Print the results */
  //printf("The points\n");
  //print_pts_grid(pts_grid);
  //printf("lengths:\n");
  //print_grid(l);
  //printf("taxinorm approximated lengths:\n");
  //print_grid(taxi_l);
  //printf("errors:\n");
  //print_grid(r);
  //printf("relative errors (taxinorm - exactnorm):\n");
  //print_grid(pr);
  //save_grid(pr, "relative_errors.data");
  plot_grid(pr);
  printf("Maximum relative error:\n");
  printf("% 8.13f\n", grid_max(pr));
  printf("Minimum relative error:\n");
  printf("% 8.13f\n", grid_min(pr));
  printf("Total relative error:\n");
  //printf("pr:\n");
  printf("% 8.13f\n", grid_max(pr) - grid_min(pr));
  //printf("compenspr:\n");
  //printf("% 8.13f\n", grid_max(compenspr) - grid_min(compenspr));
  //printf("Maximum compenspr relative error:\n");
  //printf("% 8.13f\n", grid_max(compenspr));
  //printf("Minimum compenspr relative error:\n");
  //printf("% 8.13f\n", grid_min(compenspr));
  printf("total_while_iters per call: % 13.8f\n", total_while_iters/total_calls);
  printf("total_for_iters per call: % 13.8f\n", total_for_iters/total_calls);
  return 0;
}