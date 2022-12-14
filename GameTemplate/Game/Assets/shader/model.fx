/*!
 * @brief	シンプルなモデルシェーダー。
 */
////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

//ライトの定数バッファー
cbuffer LightCb : register(b1)
{
    float3 dirDirection;
    float3 dirColor;
    float3 eyePos;
    float3 ambientLight;
}

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};

//頂点シェーダーへの入力。
struct SVSIn
{
    float4 pos      : POSITION; //モデルの頂点座標。
    float3 normal   : NORMAL; //頂点法線
    
    float3 tangent  : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv       : TEXCOORD0; //UV座標。
    
	SSkinVSIn         skinVert;	    //スキン用のデータ。
};

//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
    float3 normal       : NORMAL;       //頂点法線
	
    float3 tangent      : TANGENT;      //接ベクトル     
    float3 biNormal     : BINORMAL;     //従ベクトル。
    
    float2 uv           : TEXCOORD0; //uv座標。
    float3 worldPos     : TEXCOORD1;    // ワールド座標
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);              //アルベドマップ
Texture2D<float4> g_normalMap : register(t1);           //法線マップ
Texture2D<float4> g_specularMap : register(t2);         //スペキュラマップ

StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////
float3 CalcNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv);
float3 CalcLambertDiffuse(float3 normal);
float3 CalcPhongSpecular(float3 normal, float3 worldPos,float2 uv);


/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}else{
		m = mWorld;
	}
	psIn.pos = mul(m, vsIn.pos);
    psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);

    psIn.normal = normalize(mul(m, vsIn.normal));
    
    psIn.tangent = normalize(mul(m, vsIn.tangent));
    psIn.biNormal = normalize(mul(m, vsIn.biNormal));
    
	psIn.uv = vsIn.uv;

	return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}

/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}

/// <summary>
/// 法線を計算
/// </summary>
float3 CalcNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
    float3 binSpaceNormal = g_normalMap.Sample(g_sampler, uv).xyz;
    binSpaceNormal = (binSpaceNormal - 0.5f) * 2.0f;

    float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

    return newNormal;
}

/// <summary>
/// Lambert拡散反射を計算
/// </summary>
float3 CalcLambertDiffuse(float3 normal)
{
    float lambert = dot(-dirDirection, normal);
    lambert *= -1.0f;
    if (lambert < 0.0f)
    {
        lambert = 0.0f;
    }
    return dirColor * lambert;
}

/// <summary>
/// Phong鏡面反射を計算
/// </summary>
float3 CalcPhongSpecular(float3 normal,float3 worldPos,float2 uv)
{
    float3 refVec = reflect(dirDirection, normal);
    
    float3 toEye = eyePos - worldPos;
    toEye = normalize(toEye);
    
    float phong = dot(refVec, toEye);
    
    if (phong < 0.0f)
    {
        phong = 0.0f;
    }
    phong = pow(phong, 5.0f);
    
    float3 specularLig = dirColor * phong;
    
    return specularLig;
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain( SPSIn psIn ) : SV_Target0
{
    //アルベドカラーをサンプリング
    float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

    //法線を計算
    float3 normal = CalcNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);
    
    //ランバート拡散反射
    float3 lambert = 0.0f;
    lambert += max(0.0f, dot(normal, -dirDirection)) * dirColor;
    
    //スペキュラ鏡面反射
    float3 refVec = reflect(dirDirection, normal);
    float3 toEye = eyePos - psIn.worldPos;
    toEye = normalize(toEye);
    float t = dot(refVec, toEye);
    if (t < 0.0f)
    {
       t = 0.0f;
    }
    t = pow(t,1.0f);
    float3 specluar = dirColor * t;
    
    //スペキュラマップをサンプリング
   // float specluarPower = g_specularMap.Sample(g_sampler, psIn.uv).r;
   // specluar *= specluar * 10.0f;
    
    //拡散反射と鏡面反射と環境光を加算
    float3 lig = lambert + specluar + ambientLight;
    albedoColor.xyz *= lig;
    
    return albedoColor;
}