// @flags /O2 /Gr
/* UNKNOWN_1CABC0.CPP: cluster partitions (0x1cabc0..0x1caed0): for each
   cluster a list of the things in it, and for each thing the list of the
   clusters it touches ("cluster %s" and "%s cluster" data arrays) */

#include "unknown_11c920.h"
#include "data_array.h"

/* a thing's link to one cluster */
struct s_cluster_reference
{
	short identifier;
	byte unknown02[2];
	short cluster_index;
	byte unknown06[2];
	long next_reference_index;
};

/* a cluster's link to one thing */
struct s_data_reference
{
	short identifier;
	byte unknown02[2];
	long data_index;
	long next_reference_index;
};

struct s_cluster_partition
{
	long *cluster_first_data_references;
	s_record_pool *data_references;
	s_record_pool *cluster_references;
};

inline byte *cluster_partition_datum(s_record_pool *data, long index)
{
	return data->data + (index & 0xffff) * data->size;
}

// @retail 0x1cae40
void function_1cae40(s_cluster_partition *partition, long data_index, long *first_cluster_reference)
{
	long reference_index = *first_cluster_reference;

	while (reference_index != NONE)
	{
		s_cluster_reference *reference = (s_cluster_reference *)cluster_partition_datum(partition->cluster_references, reference_index);
		short cluster_index = reference->cluster_index;
		long next_reference_index = reference->next_reference_index;
		long *link;
		s_record_pool *data_references;

		record_pool_release(partition->cluster_references, reference_index);
		link = &partition->cluster_first_data_references[cluster_index];
		data_references = partition->data_references;
		while (*link != NONE)
		{
			s_data_reference *data_reference = (s_data_reference *)cluster_partition_datum(data_references, *link);

			if (data_reference->data_index == data_index)
			{
				long next_data_reference_index = data_reference->next_reference_index;

				record_pool_release(data_references, *link);
				*link = next_data_reference_index;
				break;
			}
			link = &data_reference->next_reference_index;
		}
		reference_index = next_reference_index;
	}
	*first_cluster_reference = NONE;
}