#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <typeinfo>
#include <cmath>
#include "split.h"


using namespace std;


// ファイルにダンプする関数
void vtk(vector<double> &data, int nx, int ny, int nz) {
	static int index = 0;
	char filename[30];
	sprintf(filename, "density%04d.vtk", index);
	ofstream ofile(filename);
	ofile << "# vtk DataFile Version 1.0" << '\n'
		<< filename << '\n'
		<< "ASCII" << '\n'
		<< "DATASET STRUCTURED_POINTS" << '\n'
		<< "DIMENSIONS " << nx << ' ' << ny << ' ' << nz << '\n'
		<< "ORIGIN 0.0 0.0 0.0" << '\n'
		<< "SPACING 1.0 1.0 1.0" << '\n'
		<< "POINT_DATA " << nx * ny * nz << '\n'
		<< "SCALARS intensity float" << '\n'
		<< "LOOKUP_TABLE default" << endl;
	for (int i = 0; i < data.size(); i++) {
		ofile << data[i] << endl;
	}
	index++;
}


// 各stepごとに全ての密度場を計算する関数
void gas_volume(double d) {
	ifstream ifile("surfactant.lammpstrj");  // 読み込むファイルのパスを指定

	const int N = 10000;
	int num_atoms;  // 粒子数
	double L;  // ボックスサイズ
	double V = pow(d, 3.0);  // セルの体積
	int Lx, Ly, Lz;  // 各次元のセルの個数
	int i_dump = 0;  // dumpfileの各行の通し番号
	int i_step = 0;  // step内でのインデックス 
	string str_dump;
	static double pos_data[N][3];  // 1stepあたりの粒子の座標データの配列
	vector<double> density;  // 密度データの配列

	while (getline(ifile, str_dump)) {  // ifileを1行ずつstr_dumpに読み込む
		if (i_dump == 3) {
			num_atoms = stoi(str_dump);  
		}
		if (i_dump == 5) {
			L = stod(split(str_dump, ' ')[1]);  // 立方体のシミュレーションボックス
			Lx = Ly = Lz = (int)(L / d);  // 一次元のセルの個数
			density.resize(Lx * Ly * Lz);  // densityをリサイズ
		}
		if (split(str_dump, ' ').size() == 5) {  // 座標データの行の場合
			i_step += 1;

			// pos_data[][]にstep内の座標データを保存
			pos_data[i_step-1][0] = stod(split(str_dump, ' ')[2]) * L;  // 元のサイズにリスケール
			pos_data[i_step-1][1] = stod(split(str_dump, ' ')[3]) * L;  
			pos_data[i_step-1][2] = stod(split(str_dump, ' ')[4]) * L;

			/////座標データから密度計算/////
			if (i_step == num_atoms) {
				fill(density.begin(), density.end(), 0);
				for (int i = 0; i < num_atoms; i++) {
					int mx = int(pos_data[i][0] / d);
					int my = int(pos_data[i][1] / d);
					int mz = int(pos_data[i][2] / d);
					if (mx >= Lx) mx -= Lx;
					if (my >= Ly) my -= Ly;
					if (mz >= Lz) mz -= Lz;
					int i_density = mx + my * Lx + mz * Lx * Ly;  // 密度データの中でのインデックス
					density[i_density] += 1.0 / V;
				}
				vtk(density, Lx, Ly, Lz);  // ファイルに密度場を出力
				i_step = 0;  // i_stepを初期化
			}
		}
		i_dump += 1;
	}
}


int main() {
	gas_volume(1.4875);
	return 0;
}
