//=============================================================================
//
// プレイヤーの処理 [player.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _PLAYER_H_
#define  _PLAYER_H_
#include "scene2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LIMIT_OPTION  (4)                    // オプションのパワーアップ限界値
#define LIMIT_PLAYER_BULLET (2)              // 同時に撃てるショットの数
#define LIMIT_PLAYER_DOBLE (2)               // 同時に撃てるショットの数(ダブル時)

#define BULLET_INTERVAL     (10)             // 弾の発射間隔
#define MISSILE_INTERVAL    (60)            // ミサイルの発射間隔
#define LASER_INTERVAL      (60)            // レーザーの発射間隔

#define INVINCIBLE_INTERVAL (60)            // 無敵時間の設定

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLife;
class COption;
class CBullet;
class CShield;
class CEnergy_UI;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENERGY (2500)                             // エネルギーの量(なくなるとミス)

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CPlayer : public CScene2D
{
public:
    // パワーアップゲージの状態
    typedef enum
    {
        POWERUP_NONE = 0,// 何もなし
        POWERUP_SPPED,   // 速うなんで！
        POWERUP_MISSILE, // ミサイルや！
        POWERUP_DOUBLE,  // 上付ダブル?
        POWERUP_LASER,   // 多い日も安心
        POWERUP_OPTION,  // 分身の術や！
        POWERUP_SHIELD,  // バ～リアッ！
        POWERUP_MAX      // ゲージ一周
    }POWERUP;

    // ショットの種類
    typedef enum
    {
        SHOTTYPE_BULLET = 0,        // 弾(パワーアップ無し)
        SHOTTYPE_DOBLE,             // 弾(ダブル状態)
        SHOTTYPE_LASER,             // レーザー
        SHOTTYPE_MAX
    }SHOTTYPE;

    // プレイヤーの状態
    typedef enum
    {
        PLAYER_STATE_NORMAL=0,       // 通常状態
        PLAYER_STATE_DAMAGE,         // ダメージ状態
        PLAYER_STATE_INVINCIBLE,     // 無敵状態
        PLAYER_STATE_MAX
    }STATE;

    CPlayer();
    ~CPlayer();

    static CPlayer *Create(void);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    void Move(void);

    void ShootBullet(void);
    void ShootDouble(void);
    void ShootLaser(void);
    void ShootMissile(void);

    void EnergyDown(void);
    void Damage(int nEnergy);
    void Miss(void);
    void PowerUp(void);
    void CheckState(void);

#if _DEBUG
     void DebugCommand(void);
#endif // _DEBUG

    // 残機のセッタ/ゲッタ
    void SetLife(int nLife) { m_nLife = nLife; }
    int GetLife(void) { return m_nLife; }

    // プレイヤーが画面に表示されているかのセッタ
    void SetAlive(bool bAlive) { m_bIsDisplayed = bAlive; }

    // プレイヤー状態のセッタ/ゲッタ
    void SetPlayerState(STATE PlayerState) { m_State = PlayerState; }
    STATE GetPlayerState(void) { return m_State; }

    // エネルギーのセッタ/ゲッタ
    void SetEnergy(int nEnergy);
    int  GetEnergy(void) { return m_nEnergy; }


    // 無敵状態のカウンタ
    void SetInvincibleCount(int nInvincibleCount) { m_nInvincibleCount = nInvincibleCount; }
    int GetInvincibleCount( void) {return m_nInvincibleCount; }

    // パワーアップゲージのセッタ/ゲッタ
    POWERUP GetPowerUp(void) { return m_powerUp; }
    void SetPowerUp(POWERUP powerUp);

    // ショットの種類のゲッタ
    SHOTTYPE GetShotType(void) { return m_shotType; }

    // ショットの発射間隔のゲッタ
    int GetInterval(void) { return m_nInterval; }

    // 弾の数のセッタ/ゲッタ
    void SetCountBullet(int nCntBullet) { m_nCntBullet = nCntBullet; }
    int GetCountBullet(void) { return m_nCntBullet; }

    // ミサイルセッタ/ゲッタ
    void SetCountMissile(int nCountMissile) { m_nCntMissile = nCountMissile; }
    int GetCountMissile(void) { return m_nCntMissile; }

    // ミサイルが使用できるかのセッタ/ ゲッタ
    void SetHasMissile(bool bHasMissile) { m_bHasMissile = bHasMissile; }
    int GetHasMissile(void) { return m_bHasMissile; }

    // オプション数のゲッタ
    int GetOptionLv(void) { return m_nOptionLv; }
    COption** GetOption(void) { return m_pOption; }

    // シールドセッタ/ゲッタ
    void SetShield(bool UsingShield) { m_bUsingShield = UsingShield; }
    bool GetShield(void) { return m_bUsingShield; }

    void SetShieldObjct(CShield* pShild) { m_pShield = pShild ;}
    CShield* GetShieldObjct(void) { return m_pShield; }

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;               // テクスチャへのポインタ

    bool                      m_bIsDisplayed;           // 画面に表示されているか
    int                       m_nLife;                  // 残機
    CLife                    *m_pLife;                  // 残機UIの情報

    STATE                     m_State;                  // プレイヤー状態
    int                       m_nEnergy;                // エネルギーのカウント
    CEnergy_UI               *m_pEnergyUI;
    int                       m_nInvincibleCount;       // 無敵状態のカウント
    int                       m_nInterval;              // ショットを撃てるまでの間隔
    SHOTTYPE                  m_shotType;               // 撃つショットのタイプ
    int                       m_nShotInterval;          // ショットの発射間隔

    POWERUP                   m_powerUp;                // パワーアップゲージの情報
    int                       m_nCntBullet;             // 撃った弾のカウント
    int                       m_nSppedLv;               // スピードのレベル
    bool                      m_bHasMissile;            // ミサイルが使用できるか
    int                       m_nCntMissile;            // 撃ったミサイルのカウント
    int                       m_nIntervalMissile;       // ミサイルの発射間隔カウント
    int                       m_nOptionLv;              // オプションのレベル
    COption                  *m_pOption[LIMIT_OPTION];  // オプションのポインタ
    bool                      m_bUsingShield;           // シールドを使用している
    CShield                  *m_pShield;                // シールドへのポインタ
};
#endif // !_PLAYER_H_
