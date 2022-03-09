

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer WorldBuffer : register( b0 )
{
	matrix World;
}

cbuffer ViewBuffer : register( b1 )
{
	matrix View;
}

cbuffer ProjectionBuffer : register( b2 )
{
	matrix Projection;
}

// マテリアルバッファ
struct MATERIAL
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	int			noTexSampling;
	float		Dummy[2];//16byte境界用
};

cbuffer MaterialBuffer : register( b3 )
{
	MATERIAL	Material;
}

// ライト用バッファ
struct LIGHT
{
	float4		Direction[5];
	float4		Position[5];
	float4		Diffuse[5];
	float4		Ambient[5];
	float4		Attenuation[5];
	int4		Flags[5];
	int			Enable;
	int			Dummy[3];//16byte境界用
};

cbuffer LightBuffer : register( b4 )
{
	LIGHT		Light;
}

struct FOG
{
	float4		Distance;
	float4		FogColor;
	int			Enable;
	float		Dummy[3];//16byte境界用
};

// フォグ用バッファ
cbuffer FogBuffer : register( b5 )
{
	FOG			Fog;
};

// 縁取り用バッファ
cbuffer Fuchi : register(b6)
{
	int			fuchi;
	int			fill[3];
};


cbuffer CameraBuffer : register(b7)
{
	float4 Camera;
}

// ライトからのビュー
cbuffer ShadowViewBuffer : register(b8)
{
	matrix ShadowView;
}

// ライトからの射影
cbuffer ProjectionBuffer : register(b9)
{
	matrix ShadowProjection;
}


//=============================================================================
// 頂点シェーダ
//=============================================================================
void VertexShaderPolygon( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0,
						  out float4 outWorldPos    : POSITION0)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	outPosition = mul(inPosition, wvp);

	outNormal = normalize(mul(float4(inNormal.xyz, 0.0f), World));

	outTexCoord = inTexCoord;

	outWorldPos = mul(inPosition, World);

	outDiffuse = inDiffuse;
}

// シャドウマップ用頂点シェーダー
void VertexShaderShadowMap( in  float4 inPosition	: POSITION0,		// 入力：ポリゴンの座標 
							in  float4 inNormal : NORMAL0,				// 入力：法線データ
							in  float4 inDiffuse : COLOR0,				// 入力：頂点の色
							in  float2 inTexCoord : TEXCOORD0,			// 入力：テクスチャの座標

							out float4 outPosition : SV_POSITION,		// 出力：スクリーン座標
							out float4 outNormal : NORMAL0,				// 出力：法線データ 
							out float4 outDepth : TEXCOORD0,			// 出力：テクスチャの座標
							out float4 outDiffuse : COLOR0,				// 出力：頂点の色
							out float4 outWorldPos : POSITION0)			// 出力：ワールド座標
{
	matrix wvp;
	wvp = mul(World, ShadowView);
	wvp = mul(wvp, ShadowProjection);
	outPosition = mul(inPosition, wvp);

	outNormal = normalize(mul(float4(inNormal.xyz, 0.0f), World));

	outDepth = outPosition;

	outWorldPos = mul(inPosition, World);

	outDiffuse = inDiffuse;
}


// 影を張り付ける頂点シェーダー
void VertexShaderShadowPaste(in  float4 inPosition        : POSITION0,
	in  float4 inNormal : NORMAL0,
	in  float4 inDiffuse : COLOR0,
	in  float2 inTexCoord : TEXCOORD0,

	out float4 outPosition : SV_POSITION,
	out float4 outNormal : NORMAL0,
	out float2 outTexCoord0 : TEXCOORD0,
	out float4 outLightPos : TEXCOORD1,
	out float4 outDiffuse : COLOR0,
	out float4 outWorldPos : POSITION0)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	outPosition = mul(inPosition, wvp);

	outNormal = normalize(mul(float4(inNormal.xyz, 0.0f), World));

	outTexCoord0 = inTexCoord;

	outWorldPos = mul(inPosition, World);

	outDiffuse = inDiffuse;

	matrix Shadowwvp;
	Shadowwvp = mul(World, ShadowView);
	Shadowwvp = mul(Shadowwvp, ShadowProjection);
	outLightPos = mul(inPosition, Shadowwvp);

}

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register( t0 );
Texture2D        g_TextureRenderTarget : register(t1);

SamplerState	g_SamplerState : register( s0 );
SamplerState    g_SamplerState3 : register(s1);

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void PixelShaderPolygon( in  float4 inPosition		: SV_POSITION,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,
						 in  float4 inWorldPos      : POSITION0,

						 out float4 outDiffuse		: SV_Target )
{
	float4 color;

	if (Material.noTexSampling == 0)
	{
		color = g_Texture.Sample(g_SamplerState, inTexCoord);

		color *= inDiffuse;
	}
	else
	{
		color = inDiffuse;
	}

	if (Light.Enable == 0)
	{
		color = color * Material.Diffuse;
	}
	else
	{
		float4 tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

		for (int i = 0; i < 5; i++)
		{
			float3 lightDir;
			float light;

			if (Light.Flags[i].y == 1)
			{
				if (Light.Flags[i].x == 1)
				{
					lightDir = normalize(Light.Direction[i].xyz);
					light = dot(lightDir, inNormal.xyz);

					light = 0.5 - 0.5 * light;
					tempColor = color * Material.Diffuse * light * Light.Diffuse[i];
				}
				else if (Light.Flags[i].x == 2)
				{
					lightDir = normalize(Light.Position[i].xyz - inWorldPos.xyz);
					light = dot(lightDir, inNormal.xyz);

					tempColor = color * Material.Diffuse * light * Light.Diffuse[i];

					float distance = length(inWorldPos - Light.Position[i]);

					float att = saturate((Light.Attenuation[i].x - distance) / Light.Attenuation[i].x);
					tempColor *= att;
				}
				else
				{
					tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
				}

				outColor += tempColor;
			}
		}

		color = outColor;
		color.a = inDiffuse.a * Material.Diffuse.a;
	}

	////フォグ
	//if (Fog.Enable == 1)
	//{
	//	float z = inPosition.z*inPosition.w;
	//	float f = (Fog.Distance.y - z) / (Fog.Distance.y - Fog.Distance.x);
	//	f = saturate(f);
	//	outDiffuse = f * color + (1 - f)*Fog.FogColor;
	//	outDiffuse.a = color.a;
	//}
	//else
	//{
		outDiffuse = color;
	//}

	//縁取り
	if (fuchi == 1)
	{
		float angle = dot(normalize(inWorldPos.xyz - Camera.xyz), normalize(inNormal));
		//if ((angle < 0.5f)&&(angle > -0.5f))
		if (angle > -0.5f)
		{
			outDiffuse.r = 1.0f;
			outDiffuse.g = 1.0f;
			outDiffuse.b = 1.0f;

		}
		//else		// ワイヤーフレームっぽくなる
		//{
		//	outDiffuse.a = 0.0f;
		//}
	}

	//// モノクロの世界
	//outDiffuse.rgb = (outDiffuse.r + outDiffuse.g + outDiffuse.b) / 3;


}

// シャドウマップを生成するピクセルシェーダー
void PixelShaderShadowMap(	 in  float4 inPosition	: SV_POSITION,
							 in  float4 inNormal	: NORMAL0,
							 in  float4 inTexCoord	: TEXCOORD0,
							 in  float4 inDiffuse	: COLOR0,
							 in  float4 inWorldPos	: POSITION0,

							out float4 outDiffuse : SV_Target)
{
	float4 color;

	// ピクセルの深度情報を計算して出力
	color = inTexCoord.z / inTexCoord.w;

	color.a = 1.0f;

	outDiffuse = color;

}




// 影を実際に描画するピクセルシェーダー
void PixelShaderShadowPaste(in  float4 inPosition: SV_POSITION,
							in  float4 inNormal : NORMAL0,
							in  float2 inTexCoord : TEXCOORD0,
							in  float4 inLightPos : TEXCOORD1,
							in  float4 inDiffuse : COLOR0,
							in  float4 inWorldPos : POSITION0,
						
							out float4 outDiffuse : SV_Target)
{
	float4 color;

	if (Material.noTexSampling == 0)
	{
		color = g_Texture.Sample(g_SamplerState, inTexCoord);

		color *= inDiffuse;
	}
	else
	{
		color = inDiffuse;
	}

	// 影処理
	// シャドウマップから値を取得
	// シャドウマップのスクリーン座標のどこのピクセル情報化を算出
	float2 shadowUV;
	shadowUV.x = 0.5f * inLightPos.x / inLightPos.w + 0.5f;
	shadowUV.y = 0.5f * inLightPos.y / inLightPos.w + 0.5f;
	shadowUV.y = 1.0f - shadowUV.y;
	shadowUV.x += 0.0008f;
	shadowUV.y += 0.0008f;

	// シャドウマップから深度(色)を抽出
	float4 shadowMap;
	shadowMap = g_TextureRenderTarget.Sample(g_SamplerState3, shadowUV);

	// 現在のピクセルの深度値を算出
	float depth = inLightPos.z / inLightPos.w;

	// シャドウマップの深度値と比較し、影を付ける
	if (depth - shadowMap.r > 0.00229f)
	{
		if (shadowMap.r > 0.0f)	color.xyz *= 0.3f;


		//if (inDiffuse.a > 0.3f)
		//{
		//color.a = 1.0f;
		//}
	}


	if (Light.Enable == 0)
	{
		color = color * Material.Diffuse;
	}
	else
	{
		float4 tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

		for (int i = 0; i < 5; i++)
		{
			float3 lightDir;
			float light;

			if (Light.Flags[i].y == 1)
			{
				if (Light.Flags[i].x == 1)
				{
					lightDir = normalize(Light.Direction[i].xyz);
					light = dot(lightDir, inNormal.xyz);

					light = 0.5 - 0.5 * light;
					tempColor = color * Material.Diffuse * light * Light.Diffuse[i];
				}
				else if (Light.Flags[i].x == 2)
				{
					lightDir = normalize(Light.Position[i].xyz - inWorldPos.xyz);
					light = dot(lightDir, inNormal.xyz);

					tempColor = color * Material.Diffuse * light * Light.Diffuse[i];

					float distance = length(inWorldPos - Light.Position[i]);

					float att = saturate((Light.Attenuation[i].x - distance) / Light.Attenuation[i].x);
					tempColor *= att;
				}
				else
				{
					tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
				}

				outColor += tempColor;
			}
		}

		color = outColor;
		color.a = inDiffuse.a * Material.Diffuse.a;
	}

	////フォグ
	//if (Fog.Enable == 1)
	//{
	//	float z = inPosition.z*inPosition.w;
	//	float f = (Fog.Distance.y - z) / (Fog.Distance.y - Fog.Distance.x);
	//	f = saturate(f);
	//	outDiffuse = f * color + (1 - f)*Fog.FogColor;
	//	outDiffuse.a = color.a;
	//}
	//else
	//{
	outDiffuse = color;
	//}

	//縁取り
	if (fuchi == 1)
	{
		float angle = dot(normalize(inWorldPos.xyz - Camera.xyz), normalize(inNormal));
		//if ((angle < 0.5f)&&(angle > -0.5f))
		if (angle > -0.5f)
		{
			outDiffuse.r = 1.0f;
			outDiffuse.g = 1.0f;
			outDiffuse.b = 1.0f;

		}
		//else		// ワイヤーフレームっぽくなる
		//{
		//	outDiffuse.a = 0.0f;
		//}
	}

	//// モノクロの世界
	//outDiffuse.rgb = (outDiffuse.r + outDiffuse.g + outDiffuse.b) / 3;


}
