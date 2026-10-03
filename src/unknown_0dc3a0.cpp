// @flags /O2 /Gr
/* UNKNOWN_0DC3A0.CPP: the game's indices of the two Havok entities of one
   of a havok component's contacts */

#include "cseries.h"
#include "globals.h"
#include "unknown_1cec30.h"

// @retail 0x0dc3a0
void havok_component_contact_properties_get(
	s_havok_component const *component,
	long contact_index,
	long *property_a,
	long *property_b)
{
	s_havok_component_element0c const *element = &component->unknown7c.data[contact_index];
	hkEntity *entity_a = element->contact->entity_a;
	hkEntity *entity_b = element->contact->entity_b;

	*property_a = havok_entity_property_get(entity_a, HAVOK_PROPERTY_2002);
	if (havok_entity_property_get(entity_b, HAVOK_PROPERTY_COMPONENT_INDEX) != NONE)
	{
		*property_b = havok_entity_property_get(entity_b, HAVOK_PROPERTY_2002);
	}
	else
	{
		*property_b = NONE;
	}
}
