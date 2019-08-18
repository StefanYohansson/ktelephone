#include "mycall.h"

#include <QDebug>

void MyCall::onCallState(OnCallStateParam &prm)
{
    CallInfo ci = getInfo();
    if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
        /* Schedule/Dispatch call deletion to another thread here */
        // del_call_scheduled = true;
        if (this->mCall)
          this->mCall->callDestroy();
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

void MyCall::setInstance(KTelephoneCall* telephoneCall)
{
  this->mCall = telephoneCall;
}
