// Fill out your copyright notice in the Description page of Project Settings.

#include "Utility/MyMath.h"

//ベクトルの長さを計算する
double get_vector_length( FVector v ) {
	return FMath::Pow( ( v.X * v.X ) + ( v.Y * v.Y ) + ( v.Z * v.Z ), 0.5 );
}

//ベクトル内積
double dot_product(FVector vl, FVector vr) {
	return vl.X * vr.X + vl.Y * vr.Y + vl.Z * vr.Z;
}

//２つのベクトルABのなす角度θを求める
double MyMath::AngleOf2Vector(FVector A, FVector B )
{
	//　※ベクトルの長さが0だと答えが出ませんので注意してください。

	//ベクトルAとBの長さを計算する
	double length_A = get_vector_length(A);
	double length_B = get_vector_length(B);

	//内積とベクトル長さを使ってcosθを求める
	double cos_sita = dot_product(A,B) / ( length_A * length_B );

	//cosθからθを求める
	double sita = FMath::Acos( cos_sita );	

	//ラジアンでなく0～180の角度でほしい場合はコメント外す
	sita = sita * 180.0 / PI;

	return sita;
}
