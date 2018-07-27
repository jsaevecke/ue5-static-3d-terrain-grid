#ifndef _GAMESPARKSRT_IRTSESSIONINTERNAL_HPP_
#define _GAMESPARKSRT_IRTSESSIONINTERNAL_HPP_

//#include <string>
#include <list>
#include <vector>
#include <memory>

#include "../../include/System/Nullable.hpp"
#include "../../include/GameSparksRT/IRTSession.hpp"
#include "../../include/GameSparksRT/IRTSessionListener.hpp"
#include "../../include/GameSparksRT/GameSparksRT.hpp"
#include "../System/String.hpp"

namespace GameSparks { namespace RT {

	class IRTSessionInternal : public IRTSession, public IRTSessionListener
	{
		public:
			virtual ~IRTSessionInternal(){}

			template <typename T0>
			void Log (const gsstl::string& tag, GameSparksRT::LogLevel level, const gsstl::string msg, const T0& v0)
			{
				auto msg_ = msg;
				replaceAll(msg_, "{0}", to_string(v0));
				this->DoLog(tag, level, msg_);
			}

			template <typename T0, typename T1>
			void Log (const gsstl::string& tag, GameSparksRT::LogLevel level, const gsstl::string msg, const T0& v0, const T1& v1)
			{
				auto msg_ = msg;
				replaceAll(msg_, "{0}", to_string(v0));
				replaceAll(msg_, "{1}", to_string(v1));
				this->DoLog(tag, level, msg_);
			}

            template <typename T0, typename T1, typename T2>
            void Log (const gsstl::string& tag, GameSparksRT::LogLevel level, const gsstl::string msg, const T0& v0, const T1& v1, const T2& v2)
            {
                auto msg_ = msg;
                replaceAll(msg_, "{0}", to_string(v0));
                replaceAll(msg_, "{1}", to_string(v1));
                replaceAll(msg_, "{2}", to_string(v2));
                this->DoLog(tag, level, msg_);
            }

			void Log (const gsstl::string& tag, GameSparksRT::LogLevel level, const gsstl::string msg)
			{
				DoLog(tag, level, msg);
			}

			virtual gsstl::string ConnectToken() const =0;
			virtual void ConnectToken(const gsstl::string&) =0;
			virtual gsstl::string FastPort() const =0;
			virtual void FastPort(const gsstl::string&) =0;
			virtual void ConnectReliable () =0;
			virtual void ConnectFast () =0;
			virtual bool ShouldExecute (int peerId, System::Nullable<int> sequence) = 0;
			virtual void SubmitAction (gsstl::unique_ptr<IRTCommand>& action) =0;
			virtual int NextSequenceNumber() = 0;

			virtual void SetConnectState(GameSparksRT::ConnectState value) = 0;
		private:
			virtual void DoLog (const gsstl::string& tag, GameSparksRT::LogLevel level, const gsstl::string& msg) = 0;

			bool replace(gsstl::string& str, const gsstl::string& from, const gsstl::string& to) {
				size_t start_pos = str.find(from);
				if(start_pos == gsstl::string::npos)
					return false;
				str.replace(start_pos, from.size(), to);
				return true;
			}

			void replaceAll(gsstl::string& str, const gsstl::string& from, const gsstl::string& to) {
				if(from.empty())
					return;
				size_t start_pos = 0;
				while((start_pos = str.find(from, start_pos)) != gsstl::string::npos) {
					str.replace(start_pos, from.size(), to);
					start_pos += to.size(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
				}
			}

			template <typename T> gsstl::string to_string(const T& v) { return System::String::ToString(v); }
            gsstl::string to_string(const gsstl::string& v) { return v; }
            gsstl::string to_string(const GameSparksRT::ConnectState& v)
            {
                switch(v)
                {
                    case GameSparksRT::ConnectState::Disconnected: return "Disconnected";
                    case GameSparksRT::ConnectState::Connecting: return "Connecting";
                    case GameSparksRT::ConnectState::ReliableAndFast: return "ReliableAndFast";
                    case GameSparksRT::ConnectState::ReliableAndFastSend: return "ReliableAndFastSend";
                    case GameSparksRT::ConnectState::ReliableOnly: return "ReliableOnly";
                    default: return "ERROR_UNKNOWN_CONNECT_STATE";
                }
            }
			template <typename T> gsstl::string to_string(const System::Nullable<T>& v)
			{
				if(!v.HasValue()) return "null";
				else return System::String::ToString(v.Value());
			}

	};

}} /* namespace GameSparks.RT */

#endif /* _GAMESPARKSRT_IRTSESSIONINTERNAL_HPP_ */
