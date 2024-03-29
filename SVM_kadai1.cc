#include <iostream>
#include <string>
#include <fstream>
#include<stdlib.h>
#include "QuadProg++.hh"
#include <cmath>
#define DATA_NUM 100
#define sigma 10.0
using namespace std;

//プロトタイプ宣言
int read_data();
//２点間の距離を出力する関数。Gaussカーネルで利用
double norm(double x_i_0,double x_i_1,double x_j_0,double x_j_1);
double PolinomicalKernel(double x_i_0,double x_i_1,double x_j_0,double x_j_1);

double GaussianKernel(double x_i_0,double x_i_1,double x_j_0,double x_j_1);
struct Dataset{
  double input_first,input_second,y;

};

int read_data() {
  return 0;
}

int main (int argc, char *const argv[]) {
  double G[MATRIX_DIM][MATRIX_DIM], g0[MATRIX_DIM], 
    CE[MATRIX_DIM][MATRIX_DIM], ce0[MATRIX_DIM], 
    CI[MATRIX_DIM][MATRIX_DIM], ci0[MATRIX_DIM], 
    alpha[MATRIX_DIM];
  double weight[2],theta;
 

  int n,m,alpha_max_number=1;	

  

  int i=0;
  double Kernel;
  n = DATA_NUM;
  string argv1;

  struct Dataset data[DATA_NUM];
  //ifstreamでファイル読み込んで、data[0]からdata[99]に格納
  /*double x[100][100];
    double y[100];
  */
  //cout << "ifstream" << endl;
  ifstream ifs("sample_circle.dat");
  string str;

  if(ifs.fail()) {
    cerr << "File do not exist.\n";
    exit(0);
  }

  while(getline(ifs, str)) {
    if(i==100) break;
    data[i].input_first=0; data[i].input_second=0; data[i].y=0;

    sscanf(str.data(), "%lf %lf %lf", &data[i].input_first, &data[i].input_second, &data[i].y);
    /*
      cout<<i<<endl;
      cout << "first = " << data[i].input_first << endl;
      cout << "second = " << data[i].input_second << endl;
      cout << "result = " <<  data[i].y << endl;
    */
    /*
      x[i][0]=data[i].input_first;
      x[i][1]=data[i].input_second;
      y[i]=data[i].y;
    */				


    i++;
  }
  //G[i][j]


  {
    {
      /*
	Gの(i,j)要素はy_i*y_j*(x_i,x_j)
	(x_i,x_j)=(1+x_i*x_j)^2=((x[i][0] ,x[i][1] ),(x[j][0],x[j][1]))


	(6 37)はx1ベクトル 
	(48 27) はx2ベクトル
	..

	-1はy1ベクトル
	-1はy2ベクトル

	x[i][0]=data[i].input_first;
	x[i][1]=data[i].input_second;
	y[i]=data[i].y;
	//G[i][j]=y_i*y_j*(x_i,x_j)
	//(x_i,x_j)=(1+x_i*x_j)^2

	*/
      for (int i = 0; i < n; i++){	
	//0のクラス出力
	if(data[i].y==-1){
	  //	cout << data[i].input_first <<"\t"<< data[i].input_second<<endl;

	}
	for (int j = 0; j < n; j++){

	  //C/C++ではargv[]はchar型
				
	  if(argv[1]){
	    argv1=argv[1];
	  }else{
	    argv1="No Kernel";
	  }
	  //	cout<<argv1<<endl;
	  if(argv1=="P"){
	    //cout<<"PolinomicalKernel"<<endl;
	    Kernel=PolinomicalKernel(data[i].input_first,data[i].input_second,data[j].input_first,data[j].input_second);


	  }else if(argv1=="G"){
	    //cout<<"GaussianKernel"<<endl;
	    Kernel=GaussianKernel(data[i].input_first,data[i].input_second,data[j].input_first,data[j].input_second);

	  }else{
	    //cout<<"NO Kernel"<<endl;
	    //内積でカーネルトリックなし
	    Kernel=(data[i].input_first*data[j].input_first+data[i].input_second*data[j].input_second);


	  }


	  G[i][j] =data[i].y*data[j].y*Kernel;

	  if(i==j) G[i][j]+=1.0e-7;



	  //cout<<"G["<<i<<"]["<<j<<"]=" << G[i][j] <<endl;

	}
	{/*
	 //シグモイドカーネル
	 Kernel=(double)tanh((1.0+data[i].input_first*data[j].input_first+data[i].input_second*data[j].input_second),2.0);
	 G[i][j] =(double)data[i].y*data[j].y*Kernel;
	 if(i==j) G[i][j]+=1.0e-7;*/
	}
      }		

    }
  }

  {//g0の要素は全て-1

    for (int i = 0; i < n; i++){
      g0[i] =(double) -1;
      //	cout<<"g0["<<i<<"]="<<g0[i] <<endl;
    }
  }
  m = 1;
  {//CEは(y1 y2 .. yn)
    for (int i = 0; i < n; i++){
      for(int j=0;j<n;j++){

	CE[i][0] =data[i].y;
	// 	cout<<"CE["<<i<<"]["<<j<<"]="<<CE[i][j]<<endl;
      }
    }



  } 


  {//ce0は0
    for (int i = 0; i < n; i++){

      ce0[i]=(double)0;



    }
  }


  {
    /*CIは
      (1 0 0 0 0 )
      (0 1 0 0 0 )
      (0 0 1 0 0 )
      (0 0 0 1 0 )
      (0 0 0 0 1 )
    */

    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
	if(i==j){
	  CI[i][j]=(double)1;
	  //cout<<"CI["<<i<<"]["<<j<<"]=" << CI[i][j] <<endl;
	}else{
	  CI[i][j]=(double)0;
	  //cout<<"CI["<<i<<"]["<<j<<"]=" << CI[i][j] <<endl;
	}


  }

  {
    //ci0は(0 0 0 0 0 )
    for (int j = 0; j < n; j++){
      ci0[j] =(double)0;
      //cout<<"ci0["<< j<<"]="<<ci0[i] <<endl;
    }
  }
  //solve_quadprogでとく


  try{
    //例外が発生する可能性のあるコード

    solve_quadprog(G, g0,100,  CE, ce0,1, CI, ci0,100, alpha);
  } catch (const std::exception& ex) {
    std::cerr << "solve_quadprog_failed" << ex.what() << std::endl;
    throw;
  }

  //  std::cout << "f: " << solve_quadprog(G, g0,DATA_NUM,  CE, ce0,1, CI, ci0,1, alpha) << std::endl;
  //std::cout << "alpha: ";
  //for (int i = 0; i < n; i++)
  //   std::cout <<"alpha["<<i<<"]"<< alpha[i] << ' ';
  //std::cout << std::endl;	


  //出力
  cout<<"alpha"<<endl;
  for ( i = 0; i <n; i++){
    if(alpha[i]>alpha[alpha_max_number]){
      alpha_max_number=i;
    }
    printf("%d\t%f\n", i, alpha[i]);//alphaは浮動小数点型で出力

  }
  //重みweight出力

  for(i=0;i<n;i++){
    weight[0]+=alpha[i]*data[i].y*data[i].input_first;
    weight[1]+=alpha[i]*data[i].y*data[i].input_second;


  }
  cout<<"weight[0]="<<weight[0]<<endl;
  cout<<"weight[1]="<<weight[1]<<endl;

  //しきい値theta出力
  //本来はどのalpha[k]のときでもthetaは同じになるはずである。ここではk=5をとる


  //(w,x)=thetaとなるときを考える(カーネルトリックなし)
  //カーネルトリックなしのときの境界の式（一次式）
  //cout<<weight[0]<<"x+"<<weight[1]<<"y="<<theta<<endl;
  if(argv[1]){
    argv1=argv[1];
  }else{
    argv1="No Kernel";
  }
  //	cout<<argv1<<endl;
  




  //alpha_max_numberはalphaが最大の番号
  cout<<"alpha_max_number"<<alpha_max_number<<endl;
  if(argv1=="P"){
    //cout<<"PolinomicalKernel"<<endl;
    Kernel=PolinomicalKernel(weight[0],weight[1],data[alpha_max_number].input_first,data[alpha_max_number].input_second);


  }else if(argv1=="G"){
    //cout<<"GaussianKernel"<<endl;
    Kernel=GaussianKernel(weight[0],weight[1],data[alpha_max_number].input_first,data[alpha_max_number].input_second);

  }else{
    //cout<<"NO Kernel"<<endl;
    //内積でカーネルトリックなし
    Kernel=(weight[0]*data[alpha_max_number].input_first+weight[1]*data[alpha_max_number].input_second);


  }
  //theta=(weight[0]*data[5].input_first+weight[1]*data[5].input_second)-data[5].y;
  theta=Kernel-data[alpha_max_number].y;
  cout<<"theta="<<theta<<endl;




  //Kernel(w,x)=thetaとなるときが境界となる
  //cout<<"ガウスカーネルを使ったときの境界の式"<<endl;
  //cout<<"exp(-("<<weight[0]<<"-x)**2-("<<weight[1]<<"-y)**2/2.0/"<<sigma<<"/"<<sigma<<")="<<theta<<endl;

  //ガウスカーネルのときに１になる点((Kernel((weight0 weight1)・(i j)-theta>0))のとき点をプロット
  if(argv1=="G"){
    for (int i = 0; i < 50; i++){
      for (int j = 0; j < 50; j++){
	//	Kernel=GaussianKernel(weight[0],weight[1],i,j);
	//	cout<<Kernel<<"and"<<theta<<endl;
	//cout<<Kernel-(double) theta<<  endl;
	//	if((Kernel-theta)>0){
	// printf("%d\t%d\t%f\t%f\n",i,j,Kernel,(Kernel-theta));
	  //   }
	//	G[i][j] =data[i].y*data[j].y*Kernel;
      }
    }
  }

  /*
  //gnuplotで描画できる2点書き出し
  printf("%f\t%f\n",0.00,theta/weight[1]);
  printf("%f\t%f\n",theta/weight[0],0.00);
  */



}
double norm(double x_i_0,double x_i_1,double x_j_0,double x_j_1){
  return pow(x_i_0 -x_j_0,2.0)+ pow(x_i_1 -x_j_1,2.0);

}
double PolinomicalKernel(double x_i_0,double x_i_1,double x_j_0,double x_j_1){
  return pow((1.0+x_i_0*x_j_0+x_i_1*x_j_1),2.0);

}

double GaussianKernel(double x_i_0,double x_i_1,double x_j_0,double x_j_1){
  return exp(-norm(x_i_0,x_i_1,x_j_0,x_j_1)/2.0/sigma/sigma);


}

/*
  double printGaussian(weight[0],weight[1],theta){
  //1000点、ガウスカーネルの境界の式をプロットする。
  for ( i = 0; i <10000; i++){
  plot[i]=;
  printf("%d\t%f\n", i, plot[i]);//alphaは浮動小数点型で出力
  }
  }*/
