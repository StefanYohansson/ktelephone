#include "mycall.h"

#include <QDebug>

void MyCall::onCallState(OnCallStateParam &prm)
{
    CallInfo ci = getInfo();
    if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
      if (this->mCall)
        this->mCall->callDestroy();
    } else if (ci.state == PJSIP_INV_STATE_CONFIRMED) {
      if (this->mCall)
        this->mCall->callbackAnswer();
    }
}

void MyCall::onCallMediaState(OnCallMediaStateParam &prm)
{
    CallInfo ci = getInfo();
    // Iterate all the call medias
    for (unsigned i = 0; i < ci.media.size(); i++) {
        if (ci.media[i].type==PJMEDIA_TYPE_AUDIO && getMedia(i)) {
            AudioMedia *aud_med = (AudioMedia *)getMedia(i);

            // Connect the call audio media to sound device
            AudDevManager& mgr = Endpoint::instance().audDevManager();
            aud_med->startTransmit(mgr.getPlaybackDevMedia());
            mgr.getCaptureDevMedia().startTransmit(*aud_med);
        }
    }
}

void MyCall::doAnswer()
{
  CallOpParam prm;
  prm.statusCode = PJSIP_SC_OK;
  this->answer(prm);
}

void MyCall::doHangup()
{
  CallOpParam prm;
  prm.statusCode = PJSIP_SC_OK;
  this->hangup(prm);
}

void MyCall::doHold(bool hold)
{
  if (!hold) {
    CallOpParam prm = new CallOpParam(hold);
    this->setHold(prm);
  } else {
    CallOpParam prm = new CallOpParam();
    prm.opt.audioCount = 1;
    prm.opt.videoCount = 0;
    prm.opt.flag = 1;
    this->reinvite(prm);
  }
}

void MyCall::doMute(bool mute)
{
  CallInfo ci = getInfo();
  // Iterate all the call medias
  for (unsigned i = 0; i < ci.media.size(); i++) {
      if (ci.media[i].type==PJMEDIA_TYPE_AUDIO && getMedia(i)) {
          AudioMedia *aud_med = (AudioMedia *)getMedia(i);

          // Connect the call audio media to sound device
          AudDevManager& mgr = Endpoint::instance().audDevManager();
          if (!mute) {
            mgr.getCaptureDevMedia().stopTransmit(*aud_med);
          } else {
            mgr.getCaptureDevMedia().startTransmit(*aud_med);
          }
      }
  }
}

void MyCall::setInstance(KTelephoneCall* telephoneCall)
{
  this->mCall = telephoneCall;
}
