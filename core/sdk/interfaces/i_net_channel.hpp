#pragma once
// used: bf_read, bf_write for buffer operations
#include "bitbuf.hpp"

#define FLOW_OUTGOING	0		
#define FLOW_INCOMING	1
#define MAX_FLOWS		2

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/inetchannelinfo.h
class i_net_channel_info
{
public:
	enum
	{
		generic = 0,	// must be first and is default group
		local_player,	// bytes for local player entity update
		other_players,	// bytes for other players update
		entities,		// all other entity bytes
		sounds,			// game sounds
		events,			// event messages
		tempents,		// temp entities
		user_messages,	// user messages
		ent_messages,	// entity messages
		voice,			// voice data
		string_table,	// a stringtable update
		move,			// client move cmds
		string_cmd,		// string command
		sign_on,		// various signondata
		paint_map,		// paintmap data
		encrypted,		// encrypted data
		total,			// must be last and is not a real group
	};

	virtual const char* get_name( ) const = 0;			// get channel name
	virtual const char* get_address( ) const = 0;			// get channel IP address as string
	virtual float		get_time( ) const = 0;			// current net time
	virtual float		get_time_connected( ) const = 0;	// get connection time in seconds
	virtual int			get_buffer_size( ) const = 0;		// netchannel packet history size
	virtual int			get_data_rate( ) const = 0;		// send data rate in byte/sec

	virtual bool		is_loopback( ) const = 0;			// true if loopback channel
	virtual bool		is_timing_out( ) const = 0;		// true if timing out
	virtual bool		is_playback( ) const = 0;			// true if demo playback
	virtual float		get_latency( int iFlow ) const = 0; // current latency (RTT), more accurate but jittering
	virtual float		get_avg_latency( int iFlow ) const = 0; // average packet latency in seconds
	virtual float		get_avg_loss( int iFlow ) const = 0; // avg packet loss[0..1]
	virtual float		get_avg_choke( int iFlow ) const = 0; // avg packet choke[0..1]
	virtual float		get_avg_data( int iFlow ) const = 0; // data flow in bytes/sec
	virtual float		get_avg_packets( int iFlow ) const = 0; // avg packets/sec
	virtual int			get_total_data( int iFlow ) const = 0; // total flow in/out in bytes
	virtual int			get_total_packets( int iFlow ) const = 0;
	virtual int			get_sequence_nr( int iFlow ) const = 0; // last send seq number
	virtual bool		is_valid_packet( int iFlow, int nFrame ) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float		get_packet_time( int iFlow, int nFrame ) const = 0; // time when packet was send
	virtual int			get_packet_bytes( int iFlow, int nFrame, int group ) const = 0; // group size of this packet
	virtual bool		get_stream_progress( int iFlow, int* piReceived, int* piTotal ) const = 0; // TCP progress if transmitting
	virtual float		get_time_since_last_received( ) const = 0; // get time since last received packet in seconds
	virtual	float		get_command_interpolation_amount( int iFlow, int nFrame ) const = 0;
	virtual void		get_packet_response_latency( int iFlow, int frame_number, int* pnLatencyMsecs, int* pnChoke ) const = 0;
	virtual void		get_remote_framerate( float* pflFrameTime, float* pflFrameTimeStdDeviation, float* pflFrameStartTimeStdDeviation ) const = 0;
	virtual float		get_timeout_seconds( ) const = 0;
};

class i_net_channel
{
public:
	std::byte	pad0[ 0x14 ];				//0x0000
	bool		bProcessingMessages;	//0x0014
	bool		bShouldDelete;			//0x0015
	bool		bStopProcessing;		//0x0016
	std::byte	pad1[ 0x1 ];				//0x0017
	int			iOutSequenceNr;			//0x0018 last send outgoing sequence number
	int			iInSequenceNr;			//0x001C last received incoming sequence number
	int			iOutSequenceNrAck;		//0x0020 last received acknowledge outgoing sequence number
	int			iOutReliableState;		//0x0024 state of outgoing reliable data (0/1) flip flop used for loss detection
	int			iInReliableState;		//0x0028 state of incoming reliable data
	int			iChokedPackets;			//0x002C number of choked packets
	std::byte	pad2[ 0x414 ];			//0x0030

	int	send_datagram( bf_write* pDatagram )
	{
		return utils::call_vfunc<int>( this, 46, pDatagram );
	}

	bool Transmit( bool bOnlyReliable = false )
	{
		return utils::call_vfunc<bool>( this, 49, bOnlyReliable );
	}
}; // Size: 0x0444

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/inetmessage.h
class i_net_message
{
public:
	virtual					~i_net_message( ) { }
	virtual void			set_net_channel( void* pNetChannel ) = 0;
	virtual void			set_reliable( bool bState ) = 0;
	virtual bool			process( ) = 0;
	virtual	bool			read_from_buffer( bf_read& buffer ) = 0;
	virtual	bool			write_to_buffer( bf_write& buffer ) = 0;
	virtual bool			is_reliable( ) const = 0;
	virtual int				get_type( ) const = 0;
	virtual int				get_group( ) const = 0;
	virtual const char* get_name( ) const = 0;
	virtual i_net_channel* get_net_channel( ) const = 0;
	virtual const char* to_string( ) const = 0;
	virtual std::size_t		get_size( ) const = 0;
};

class cclc_msg_move
{
private:
	std::byte pad0[ 0x8 ];
public:
	int nBackupCommands;
	int nNewCommands;
};

template <typename T>
class c_net_message_pb : public i_net_message, public T { };
using cclc_msg_move_t = c_net_message_pb<cclc_msg_move>;

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/inetmsghandler.h
class i_net_channel_handler
{
public:
	virtual			~i_net_channel_handler( ) { }
	virtual void	connection_start( i_net_channel* pChannel ) = 0; // called first time network channel is established
	virtual void	connection_stop( ) = 0; // called first time network channel is established
	virtual void	connection_closing( const char* szReason ) = 0; // network channel is being closed by remote site
	virtual void	connection_crashed( const char* szReason ) = 0; // network error occured
	virtual void	packet_start( int iIncomingSequence, int iOutgoingAcknowledged ) = 0;	// called each time a new packet arrived
	virtual void	packet_end( ) = 0; // all messages has been parsed
	virtual void	file_requested( const char* szFileName, unsigned int uTransferID, bool bReplayDemoFile ) = 0; // other side request a file for download
	virtual void	file_received( const char* szFileName, unsigned int uTransferID, bool bReplayDemoFile ) = 0; // we received a file
	virtual void	file_denied( const char* szFileName, unsigned int uTransferID, bool bReplayDemoFile ) = 0; // a file request was denied by other side
	virtual void	file_sent( const char* szFileName, unsigned int uTransferID, bool bReplayDemoFile ) = 0; // we sent a file
	virtual bool	change_splitscreen_user( int nSplitScreenUserSlot ) = 0; // interleaved networking used by SS system is changing the SS player slot that the subsequent messages pertain to
};
