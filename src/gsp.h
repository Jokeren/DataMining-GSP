#ifndef GSP_H
#define GSP_H

#include "common.h"

namespace gsp {
	class Seperator;

	struct item_t {
		uint32_t id;

		bool operator < (const struct item_t& t) const {
			return id < t.id;
		}
	};

	//To-do: this is not a good style, use composite instead if you have time
	struct item_set_t: public std::set<struct item_t> {
		uint32_t timestamp;

		bool operator < (const struct item_set_t& t) const {
			return timestamp < t.timestamp;
		}
	};

	typedef struct item_set_t item_set;
	typedef std::vector<item_set> events;

	//To-do: seperate these classes
	class Sequence: public events {
		public:
			Sequence():_m_sequence_id(0) {
			}

			Sequence(const uint32_t sequence_id) : _m_sequence_id(sequence_id) {
			}

			void set_sequence_id(const uint32_t sequence_id) {
				_m_sequence_id = sequence_id;
			}

			uint32_t get_sequence_id() const {
				return _m_sequence_id;
			}	

			void make_list();

			size_t get_unique_size() const {
				return _m_items_time_list.size();
			}

			int32_t get_timestamp(uint32_t id, uint32_t idx) const;

		private:
			uint32_t _m_sequence_id;
			std::map<uint32_t, std::vector<uint32_t> > _m_items_time_list;
	};

	class Pattern: public events {
		public:
			Pattern() : _m_support(0), _m_length(0) {}

			Pattern(uint32_t support, uint32_t length) : 
				_m_support(support), _m_length(length) {}

			uint32_t get_support() const {
				return _m_support;
			}

			void set_support(uint32_t support) {
				_m_support = support;
			}

			void increase_support() {
				_m_support++;
			}

			uint32_t get_length() const {
				return _m_length;
			}

			void set_length(uint32_t length) {
				_m_length = length;
			}

			const char* c_str(); 

			const vector<uint32_t> items_list();

		private:
			void transform();

			void transform_to_list();

		private:
			uint32_t _m_support;
			uint32_t _m_length;
			std::string _m_pattern;
			std::vector<uint32_t> _m_items;
	};

	class Gsp {
		public:
			Gsp() {
			}
//			fix: not pass seperator, this is ambiguous
//			Gsp(char *file_path, uint32_t support, uint32_t nsequences,
//					uint32_t nevents, uint32_t min_gap, uint32_t max_gap, Seperator *seperator):
//	   			_m_file_path(file_path), min_support(support), _m_nsequences(nsequences), 
//				_m_nevents(nevents), _m_min_gap(min_gap), _m_max_gap(max_gap), _m_seperator(seperator) {
//			}
//
//			void execute(char *file_path, uint32_t support, uint32_t nsequences,
//					uint32_t nevents, uint32_t min_gap, uint32_t max_gap, Seperator *seperator); 

			Gsp(const char *file_path, uint32_t support, uint32_t nsequences,
					uint32_t nevents, uint32_t min_gap, uint32_t max_gap, const char *seperator_type):
	   			_m_file_path(file_path), _m_support(support), _m_nsequences(nsequences), 
				_m_nevents(nevents), _m_min_gap(min_gap), _m_max_gap(max_gap), _m_seperator_type(seperator_type) {
			}

			void execute(const char *file_path, uint32_t support, uint32_t nsequences,
					uint32_t nevents, uint32_t min_gap, uint32_t max_gap, const char *seperator_type, GspFileType file_type); 

		private:
			bool generate_candidates(uint32_t k);

			void count_support(uint32_t k);

			void read_input_normal(const input& stream);

			void read_input_spmf(const input& stream);

			void print_patterns(uint32_t k);

		private:
			Seperator *_m_seperator;
			uint32_t _m_support;
			uint32_t _m_min_gap;
			uint32_t _m_max_gap;
			uint32_t _m_nevents;
			uint32_t _m_nsequences;
			const char *_m_file_path;
			const char *_m_seperator_type;
	};

}//namespace gsp
#endif // GSP_H
