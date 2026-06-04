#include "Actor.h"
#include "ActorFrame.h"
#include "RageDisplay.h"
#include "RageUtil.h"
#include "RageMath.h"
#include "RageLog.h"
#include "XmlFile.h"
#include "LuaBinding.h"
#include "ThemeManager.h"
#include "LuaReference.h"
#include "MessageManager.h"
#include "LightsManager.h" // for NUM_CabinetLight
#include "ActorUtil.h"
#include "Preference.h"

#include <cmath>
#include <cstddef>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <string>
>>>>>>> release
=======
>>>>>>> Stashed changes
/* XXX: This calls InitCommand, which must happen after all other
 * initialization (eg. ActorFrame loading children).  However, it
 * also loads input variables, which should happen first.  The
 * former is more important. */
void Actor::LoadFromNode( const XNode* pNode )
{
	Lua *L = LUA->Get();
	FOREACH_CONST_Attr( pNode, pAttr )
	{
		// Load Name, if any.
		const RString &sKeyName = pAttr->first;
		const XNodeValue *pValue = pAttr->second;
		if( EndsWith(sKeyName,"Command") )
		{
			LuaReference *pRef = new LuaReference;
			pValue->PushValue( L );
			pRef->SetFromStack( L );
			RString sCmdName = sKeyName.Left( sKeyName.size()-7 );
			AddCommand( sCmdName, apActorCommands( pRef ) );
		}
		else if( sKeyName == "Name" )			SetName( pValue->GetValue<RString>() );
		else if( sKeyName == "BaseRotationX" )		SetBaseRotationX( pValue->GetValue<float>() );
		else if( sKeyName == "BaseRotationY" )		SetBaseRotationY( pValue->GetValue<float>() );
		else if( sKeyName == "BaseRotationZ" )		SetBaseRotationZ( pValue->GetValue<float>() );
		else if( sKeyName == "BaseZoomX" )		SetBaseZoomX( pValue->GetValue<float>() );
		else if( sKeyName == "BaseZoomY" )		SetBaseZoomY( pValue->GetValue<float>() );
		else if( sKeyName == "BaseZoomZ" )		SetBaseZoomZ( pValue->GetValue<float>() );
	}

  LUA->Release(L);

  // Don't recurse Init.  It gets called once for every Actor when the
  // Actor is loaded, and we don't want to call it again.
  PlayCommandNoRecurse(Message("Init"));
}


		bool bBlinkOn = fPercentThroughEffect > 0.5f;
		float fPercentBetweenColors = RageFastSin( (fPercentThroughEffect + 0.25f) * 2 * PI ) / 2 + 0.5f;
		ASSERT_M( fPercentBetweenColors >= 0 && fPercentBetweenColors <= 1,
			ssprintf("PercentBetweenColors: %f, PercentThroughEffect: %f", fPercentBetweenColors, fPercentThroughEffect) );
		float fOriginalAlpha = m_current_with_effects.diffuse[0].a;

		// todo: account for SSC_FUTURES -aj
		switch( m_Effect )
		{
		case diffuse_blink:
			/* XXX: Should diffuse_blink and diffuse_shift multiply the m_current_with_effects color?
			 * (That would have the same effect with 1,1,1,1, and allow tweening the diffuse
			 * while blinking and shifting.) */
			for(int i=0; i<NUM_DIFFUSE_COLORS; i++)
			{
				m_current_with_effects.diffuse[i] = bBlinkOn ? m_effectColor1 : m_effectColor2;
				m_current_with_effects.diffuse[i].a *= fOriginalAlpha;	// multiply the alphas so we can fade even while an effect is playing
			}
			break;
		case diffuse_shift:
			for(int i=0; i<NUM_DIFFUSE_COLORS; i++)
			{
				m_current_with_effects.diffuse[i] = m_effectColor1*fPercentBetweenColors + m_effectColor2*(1.0f-fPercentBetweenColors);
				m_current_with_effects.diffuse[i].a *= fOriginalAlpha;	// multiply the alphas so we can fade even while an effect is playing
			}
			break;
		case diffuse_ramp:
			for(int i=0; i<NUM_DIFFUSE_COLORS; i++)
			{
				m_current_with_effects.diffuse[i] = m_effectColor1*fPercentThroughEffect + m_effectColor2*(1.0f-fPercentThroughEffect);
				m_current_with_effects.diffuse[i].a *= fOriginalAlpha;	// multiply the alphas so we can fade even while an effect is playing
			}
			break;
		case glow_blink:
			m_current_with_effects.glow = bBlinkOn ? m_effectColor1 : m_effectColor2;
			m_current_with_effects.glow.a *= fOriginalAlpha;	// don't glow if the Actor is transparent!
			break;
		case glow_shift:
			m_current_with_effects.glow = m_effectColor1*fPercentBetweenColors + m_effectColor2*(1.0f-fPercentBetweenColors);
			m_current_with_effects.glow.a *= fOriginalAlpha;	// don't glow if the Actor is transparent!
			break;
		case glow_ramp:
			m_current_with_effects.glow = m_effectColor1*fPercentThroughEffect + m_effectColor2*(1.0f-fPercentThroughEffect);
			m_current_with_effects.glow.a *= fOriginalAlpha;	// don't glow if the Actor is transparent!
			break;
		case rainbow:
			m_current_with_effects.diffuse[0] = RageColor(
				RageFastCos( fPercentBetweenColors*2*PI ) * 0.5f + 0.5f,
				RageFastCos( fPercentBetweenColors*2*PI + PI * 2.0f / 3.0f ) * 0.5f + 0.5f,
				RageFastCos( fPercentBetweenColors*2*PI + PI * 4.0f / 3.0f) * 0.5f + 0.5f,
				fOriginalAlpha );
			for( int i=1; i<NUM_DIFFUSE_COLORS; i++ )
				m_current_with_effects.diffuse[i] = m_current_with_effects.diffuse[0];
			break;
		case wag:
			m_current_with_effects.rotation += m_vEffectMagnitude * RageFastSin( fPercentThroughEffect * 2.0f * PI );
			break;
		case spin:
			// nothing needs to be here
			break;
		case vibrate:
			m_current_with_effects.pos.x += m_vEffectMagnitude.x * randomf(-1.0f, 1.0f) * GetZoom();
			m_current_with_effects.pos.y += m_vEffectMagnitude.y * randomf(-1.0f, 1.0f) * GetZoom();
			m_current_with_effects.pos.z += m_vEffectMagnitude.z * randomf(-1.0f, 1.0f) * GetZoom();
			break;
		case bounce:
			{
				float fPercentOffset = RageFastSin( fPercentThroughEffect*PI );
				m_current_with_effects.pos += m_vEffectMagnitude * fPercentOffset;
			}
			break;
		case bob:
			{
				float fPercentOffset = RageFastSin( fPercentThroughEffect*PI*2 );
				m_current_with_effects.pos += m_vEffectMagnitude * fPercentOffset;
			}
			break;
		case pulse:
			{
				float fMinZoom = m_vEffectMagnitude[0];
				float fMaxZoom = m_vEffectMagnitude[1];
				float fPercentOffset = RageFastSin( fPercentThroughEffect*PI );
				float fZoom = SCALE( fPercentOffset, 0.f, 1.f, fMinZoom, fMaxZoom );
				m_current_with_effects.scale *= fZoom;

          // Use the color as a Vector3 to scale the effect for added control.
          RageColor c =
              SCALE(fPercentOffset, 0.f, 1.f, m_effectColor1, m_effectColor2);
          m_current_with_effects.scale.x *= c.r;
          m_current_with_effects.scale.y *= c.g;
          m_current_with_effects.scale.z *= c.b;
        }
        break;
      default:
        FAIL_M(ssprintf("Invalid effect: %i", m_Effect));
    }
  }

  if (m_fBaseAlpha != 1) {
    m_internalDiffuse.a *= m_fBaseAlpha;
  }

  if (m_internalDiffuse != RageColor(1, 1, 1, 1)) {
    if(m_pTempState != &m_current_with_effects) {
      m_pTempState = &m_current_with_effects;
      m_current_with_effects = m_current;
    }

    for (int i = 0; i < NUM_DIFFUSE_COLORS; ++i) {
      m_current_with_effects.diffuse[i] *= m_internalDiffuse;
    }
  }

  if (m_internalGlow.a > 0) {
    if(m_pTempState != &m_current_with_effects) {
      m_pTempState = &m_current_with_effects;
      m_current_with_effects = m_current;
    }

    // Blend using Screen mode
    m_current_with_effects.glow =
        (m_current_with_effects.glow + m_internalGlow -
         m_internalGlow * m_current_with_effects.glow);
  }
}

void Actor::BeginDraw() // set the world matrix
{
	DISPLAY->PushMatrix();

	if( m_pTempState->pos.x != 0 || m_pTempState->pos.y != 0 || m_pTempState->pos.z != 0 )
	{
		RageMatrix m;
		RageMatrixTranslate(
			&m,
			m_pTempState->pos.x,
			m_pTempState->pos.y,
			m_pTempState->pos.z
			);
		DISPLAY->PreMultMatrix( m );
	}

	{
		/* The only time rotation and quat should normally be used simultaneously
		 * is for m_baseRotation. Most objects aren't rotated at all, so optimize
		 * that case. */
		const float fRotateX = m_pTempState->rotation.x + m_baseRotation.x;
		const float fRotateY = m_pTempState->rotation.y + m_baseRotation.y;
		const float fRotateZ = m_pTempState->rotation.z + m_baseRotation.z;

		if( fRotateX != 0 || fRotateY != 0 || fRotateZ != 0 )
		{
			RageMatrix m;
			RageMatrixRotationXYZ( &m, fRotateX, fRotateY, fRotateZ );
			DISPLAY->PreMultMatrix( m );
		}
	}

	// handle scaling
	{
		const float fScaleX = m_pTempState->scale.x * m_baseScale.x;
		const float fScaleY = m_pTempState->scale.y * m_baseScale.y;
		const float fScaleZ = m_pTempState->scale.z * m_baseScale.z;

		if( fScaleX != 1 || fScaleY != 1 || fScaleZ != 1 )
		{
			RageMatrix m;
			RageMatrixScale(
				&m,
				fScaleX,
				fScaleY,
				fScaleZ );
			DISPLAY->PreMultMatrix( m );
		}
	}

	// handle alignment; most actors have default alignment.
	if( unlikely(m_fHorizAlign != 0.5f || m_fVertAlign != 0.5f) )
	{
		float fX = SCALE( m_fHorizAlign, 0.0f, 1.0f, +m_size.x/2.0f, -m_size.x/2.0f );
		float fY = SCALE( m_fVertAlign, 0.0f, 1.0f, +m_size.y/2.0f, -m_size.y/2.0f );
		RageMatrix m;
		RageMatrixTranslate(
			&m,
			fX,
			fY,
			0
			);
		DISPLAY->PreMultMatrix( m );
	}

	if( m_pTempState->quat.x != 0 ||  m_pTempState->quat.y != 0 ||  m_pTempState->quat.z != 0 || m_pTempState->quat.w != 1 )
	{
		RageMatrix mat;
		RageMatrixFromQuat( &mat, m_pTempState->quat );

		DISPLAY->MultMatrix(mat);
	}

	// handle skews
	if( m_pTempState->fSkewX != 0 )
	{
		DISPLAY->SkewX( m_pTempState->fSkewX );
	}

	if( m_pTempState->fSkewY != 0 )
	{
		DISPLAY->SkewY( m_pTempState->fSkewY );
	}

	if( m_texTranslate.x != 0 || m_texTranslate.y != 0 )
	{
		DISPLAY->TexturePushMatrix();
		DISPLAY->TextureTranslate( m_texTranslate.x, m_texTranslate.y );
	}

=======
    m_current_with_effects.glow = m_current_with_effects.glow + m_internalGlow -
                                  m_internalGlow * m_current_with_effects.glow;
  }
}

void Actor::SetGlobalRenderStates() {
  // set Actor-defined render states
  if (!g_bShowMasks.Get() || m_BlendMode != BLEND_NO_EFFECT) {
    DISPLAY->SetBlendMode(m_BlendMode);
  }
  DISPLAY->SetZWrite(m_bZWrite);
  DISPLAY->SetZTestMode(m_ZTestMode);

  // BLEND_NO_EFFECT is used to draw masks to the Z-buffer, which always wants
  // Z-bias enabled.
  if (m_fZBias == 0 && m_BlendMode == BLEND_NO_EFFECT) {
    DISPLAY->SetZBias(1.0f);
  } else {
    DISPLAY->SetZBias(m_fZBias);
  }

  if (m_bClearZBuffer) {
    DISPLAY->ClearZBuffer();
  }
  DISPLAY->SetCullMode(m_CullMode);
}

void Actor::SetTextureRenderStates() {
  DISPLAY->SetTextureWrapping(TextureUnit_1, m_bTextureWrapping);
  DISPLAY->SetTextureFiltering(TextureUnit_1, m_bTextureFiltering);
}

void Actor::EndDraw() {
  DISPLAY->PopMatrix();

  if (m_texTranslate.x != 0 || m_texTranslate.y != 0) {
    DISPLAY->TexturePopMatrix();
  }
}

void Actor::CalcPercentThroughTween()
{
	TweenState &TS = m_Tweens[0]->state;
	TweenInfo  &TI = m_Tweens[0]->info;
	const float percent_through = 1-(TI.m_fTimeLeftInTween / TI.m_fTweenTime);
	// distort the percentage if appropriate
	float percent_along = TI.m_pTween->Tween(percent_through);
	TweenState::MakeWeightedAverage(m_current, m_start, TS, percent_along);
	UpdatePercentThroughTween(percent_along);
}

void Actor::UpdateTweening( float fDeltaTime )
{
	if(fDeltaTime < 0.0 && !m_Tweens.empty())
	{
		m_Tweens[0]->info.m_fTimeLeftInTween-= fDeltaTime;
		CalcPercentThroughTween();
		return;
	}
	while( !m_Tweens.empty() // something to do
		&& fDeltaTime > 0 )	// something will change
	{
		// update current tween state
		// earliest tween
		TweenState &TS = m_Tweens[0]->state;
		TweenInfo  &TI = m_Tweens[0]->info;

    bool bBeginning = TI.m_fTimeLeftInTween == TI.m_fTweenTime;

		float fSecsToSubtract = std::min( TI.m_fTimeLeftInTween, fDeltaTime );
		TI.m_fTimeLeftInTween -= fSecsToSubtract;
		fDeltaTime -= fSecsToSubtract;

		RString sCommand = TI.m_sCommandName;
		if( bBeginning )			// we are just beginning this tween
		{
			m_start = m_current;	// set the start position
			SetCurrentTweenStart();
		}

		if( TI.m_fTimeLeftInTween == 0 )	// Current tween is over.  Stop.
		{
			m_current = TS;

			// delete the head tween
			delete m_Tweens.front();
			m_Tweens.erase( m_Tweens.begin() );
			EraseHeadTween();
		}
		else	// in the middle of tweening. Recalcute the current position.
		{
			CalcPercentThroughTween();
		}

		if( bBeginning )
		{
			// Execute the command in this tween (if any). Do this last, and don't
			// access TI or TS after, since this may modify the tweening queue.
			if( !sCommand.empty() )
			{
				if( sCommand.Left(1) == "!" )
					MESSAGEMAN->Broadcast( sCommand.substr(1) );
				else
					this->PlayCommand( sCommand );
			}
		}
	}
}

bool Actor::IsFirstUpdate() const { return m_bFirstUpdate; }

void Actor::Update(float fDeltaTime) {
  //	LOG->Trace( "Actor::Update( %f )", fDeltaTime );

  float rate = GameLoop::GetUpdateRate();
  if (rate != 1 && !rate_scaling_enabled_) {
    // Prevent divide by 0 when tab + tilde are both pressed.
    if (rate != 0) {
      fDeltaTime *= (1 / rate);
    }
  }

  ASSERT_M(fDeltaTime >= 0, ssprintf("DeltaTime: %f", fDeltaTime));

  if (m_fHibernateSecondsLeft > 0) {
    m_fHibernateSecondsLeft -= fDeltaTime;
    if (m_fHibernateSecondsLeft > 0) {
      return;
    }

    // Grab the leftover time.
    fDeltaTime = -m_fHibernateSecondsLeft;
    m_fHibernateSecondsLeft = 0;
  }
  for (size_t i = 0; i < m_WrapperStates.size(); ++i) {
    m_WrapperStates[i]->Update(fDeltaTime);
  }

  this->UpdateInternal(fDeltaTime);
}

void Actor::Update( float fDeltaTime )
{
//	LOG->Trace( "Actor::Update( %f )", fDeltaTime );
	ASSERT_M( fDeltaTime >= 0, ssprintf("DeltaTime: %f",fDeltaTime) );

	if( m_fHibernateSecondsLeft > 0 )
	{
		m_fHibernateSecondsLeft -= fDeltaTime;
		if( m_fHibernateSecondsLeft > 0 )
		{
			return;
		}

		// Grab the leftover time.
		fDeltaTime = -m_fHibernateSecondsLeft;
		m_fHibernateSecondsLeft = 0;
	}
	for(std::size_t i= 0; i < m_WrapperStates.size(); ++i)
	{
		m_WrapperStates[i]->Update(fDeltaTime);
	}

	this->UpdateInternal( fDeltaTime );
}

void Actor::UpdateInternal(float delta_time) {
  if (m_bFirstUpdate) {
    m_bFirstUpdate = false;
  }

  const float effectPeriod = GetEffectPeriod();
  switch (m_EffectClock) {
    case CLOCK_TIMER:
      m_fSecsIntoEffect += delta_time;
      m_fEffectDelta = delta_time;
      // Wrap the counter, so it doesn't increase indefinitely (causing loss
      // of precision if a screen is left to sit for a day).
      if (m_fSecsIntoEffect > effectPeriod) {
        m_fSecsIntoEffect -= effectPeriod;
      }
      break;
    case CLOCK_TIMER_GLOBAL:
      generic_global_timer_update(
          RageTimer::GetTimeSinceStartMicroseconds(), m_fEffectDelta,
          m_fSecsIntoEffect);
      break;
    case CLOCK_BGM_BEAT:
      generic_global_timer_update(
          g_fCurrentBGMBeat, m_fEffectDelta, m_fSecsIntoEffect);
      break;
    case CLOCK_BGM_BEAT_PLAYER1:
      generic_global_timer_update(
          g_vfCurrentBGMBeatPlayer[PLAYER_1], m_fEffectDelta,
          m_fSecsIntoEffect);
      break;
    case CLOCK_BGM_BEAT_PLAYER2:
      generic_global_timer_update(
          g_vfCurrentBGMBeatPlayer[PLAYER_2], m_fEffectDelta,
          m_fSecsIntoEffect);
      break;
    case CLOCK_BGM_TIME:
      generic_global_timer_update(
          g_fCurrentBGMTime, m_fEffectDelta, m_fSecsIntoEffect);
      break;
    case CLOCK_BGM_BEAT_NO_OFFSET:
      generic_global_timer_update(
          g_fCurrentBGMBeatNoOffset, m_fEffectDelta, m_fSecsIntoEffect);
      break;
    case CLOCK_BGM_TIME_NO_OFFSET:
      generic_global_timer_update(
          g_fCurrentBGMTimeNoOffset, m_fEffectDelta, m_fSecsIntoEffect);
      break;
    default:
      if (m_EffectClock >= CLOCK_LIGHT_1 && m_EffectClock <= CLOCK_LIGHT_LAST) {
        int lightIndex = m_EffectClock - CLOCK_LIGHT_1;
        generic_global_timer_update(
            g_fCabinetLights[lightIndex], m_fEffectDelta, m_fSecsIntoEffect);
      }
      break;
  }

  // update effect
  switch (m_Effect) {
    case spin:
      m_current.rotation += m_fEffectDelta * m_vEffectMagnitude;
      wrap(m_current.rotation.x, 360);
      wrap(m_current.rotation.y, 360);
      wrap(m_current.rotation.z, 360);
      break;
    default:
      break;
  }

  if (m_tween_uses_effect_delta) {
    delta_time = m_fEffectDelta;
  }
  this->UpdateTweening(delta_time);
}

void Actor::UpdateInternal(float delta_time)
{
	if( m_bFirstUpdate )
		m_bFirstUpdate = false;

	switch(m_EffectClock)
	{
		case CLOCK_TIMER:
			m_fSecsIntoEffect+= delta_time;
			m_fEffectDelta= delta_time;
			// Wrap the counter, so it doesn't increase indefinitely (causing loss
			// of precision if a screen is left to sit for a day).
			if(m_fSecsIntoEffect > GetEffectPeriod())
			{
				m_fSecsIntoEffect-= GetEffectPeriod();
			}
			break;
		case CLOCK_TIMER_GLOBAL:
			generic_global_timer_update(RageTimer::GetTimeSinceStart(),
				m_fEffectDelta, m_fSecsIntoEffect);
			break;
		case CLOCK_BGM_BEAT:
			generic_global_timer_update(g_fCurrentBGMBeat,
				m_fEffectDelta, m_fSecsIntoEffect);
			break;
		case CLOCK_BGM_BEAT_PLAYER1:
			generic_global_timer_update(g_vfCurrentBGMBeatPlayer[PLAYER_1],
				m_fEffectDelta, m_fSecsIntoEffect);
			break;
		case CLOCK_BGM_BEAT_PLAYER2:
			generic_global_timer_update(g_vfCurrentBGMBeatPlayer[PLAYER_2],
				m_fEffectDelta, m_fSecsIntoEffect);
			break;
		case CLOCK_BGM_TIME:
			generic_global_timer_update(g_fCurrentBGMTime,
				m_fEffectDelta, m_fSecsIntoEffect);
			break;
		case CLOCK_BGM_BEAT_NO_OFFSET:
			generic_global_timer_update(g_fCurrentBGMBeatNoOffset,
				m_fEffectDelta, m_fSecsIntoEffect);
			break;
		case CLOCK_BGM_TIME_NO_OFFSET:
			generic_global_timer_update(g_fCurrentBGMTimeNoOffset,
				m_fEffectDelta, m_fSecsIntoEffect);
			break;
		default:
			if(m_EffectClock >= CLOCK_LIGHT_1 && m_EffectClock <= CLOCK_LIGHT_LAST)
			{
				generic_global_timer_update(
					g_fCabinetLights[m_EffectClock - CLOCK_LIGHT_1],
					m_fEffectDelta, m_fSecsIntoEffect);
			}
			break;
	}

	// update effect
	// todo: account for SSC_FUTURES -aj
	switch( m_Effect )
	{
		case spin:
			m_current.rotation += m_fEffectDelta*m_vEffectMagnitude;
			wrap( m_current.rotation.x, 360 );
			wrap( m_current.rotation.y, 360 );
			wrap( m_current.rotation.z, 360 );
			break;
		default: break;
	}

	if(m_tween_uses_effect_delta)
	{
		delta_time= m_fEffectDelta;
	}
	this->UpdateTweening(delta_time);
}

RString Actor::GetLineage() const
{
	RString sPath;

	if( m_pParent )
		sPath = m_pParent->GetLineage() + '/';
	sPath += ssprintf( "<type %s> %s", typeid(*this).name(), m_sName.c_str() );
	return sPath;
}

void Actor::RemoveWrapperState(size_t i) {
  ASSERT(i < m_WrapperStates.size());
  RageUtil::SafeDelete(m_WrapperStates[i]);
  m_WrapperStates.erase(m_WrapperStates.begin() + i);
}

=======
>>>>>>> Stashed changes

    ADD_METHOD(Draw);
  }
};

LUA_REGISTER_INSTANCED_BASE_CLASS(Actor)
// lua end

/*
 * (c) 2001-2004 Chris Danford
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
