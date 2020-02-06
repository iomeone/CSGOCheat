#include "HookManager.h"

#include "Menu.h"

SendDatagramFn hooks::oSendDatagram;
std::deque<CIncomingSequence> HookValues::sequences;
int HookValues::lastincomingsequencenumber;

void hooks::UpdateIncomingSequences()
{
	if (game_interfaces::ClientState->m_NetChannel()->m_nInSequenceNr > HookValues::lastincomingsequencenumber)
	{
		HookValues::lastincomingsequencenumber = game_interfaces::ClientState->m_NetChannel()->m_nInSequenceNr;
		HookValues::sequences.push_front(CIncomingSequence(game_interfaces::ClientState->m_NetChannel()->m_nInReliableState, game_interfaces::ClientState->m_NetChannel()->m_nOutReliableState, game_interfaces::ClientState->m_NetChannel()->m_nInSequenceNr, game_interfaces::Globals->realtime));
	}

	if (HookValues::sequences.size() > 2048)
	{
		HookValues::sequences.pop_back();
	}
}

void AddLatencyToNetchan(INetChannel *netchan, float Latency)
{
	for (CIncomingSequence& seq : HookValues::sequences)
	{
		if ((game_interfaces::Globals->realtime - seq.curtime) >= Latency)
		{
			netchan->m_nInReliableState = seq.inreliablestate;
			netchan->m_nInSequenceNr = seq.sequencenr;
			break;
		}
	}
}

int __fastcall hooks::SendDatagram_Hook(INetChannel* netchan, void*, bf_write* datagram)
{
	if (game_interfaces::Engine->InGame() && game_interfaces::Engine->Connected())
	{
		if (MenuControls::m_fakelatency->GetValue() < 1 || datagram)
		{
			return hooks::oSendDatagram(netchan, datagram);
		}

		int instate = netchan->m_nInReliableState;
		int insequencenr = netchan->m_nInSequenceNr;

		float ping = 0.f;
		INetChannelInfo* net_channel = game_interfaces::Engine->GetNetChannelInfo();
		if (net_channel)
		{
			ping = net_channel->GetLatency(FLOW_OUTGOING) * 1000.f;
		}

		float to_add = (float)MenuControls::m_fakelatency->GetValue() - ping;

		if (to_add > 0.f)
		{
			AddLatencyToNetchan(netchan, (to_add / 1000.f));
		}

		int ret = oSendDatagram(netchan, datagram);

		netchan->m_nInReliableState = instate;
		netchan->m_nInSequenceNr = insequencenr;

		return ret;
	}
	return oSendDatagram(netchan, datagram);
}