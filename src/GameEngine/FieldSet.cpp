#include "FieldSet.h"

std::vector<Field*> FieldSet::get()
{
  return field;
}

Field* FieldSet::getLastElement()
{
  return field.back();
}

bool FieldSet::empty()
{
  return (field.empty());
}

void FieldSet::clear()
{
  field.clear();
  hightlighOnFields.clear();
}


void FieldSet::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  for (auto & h : hightlighOnFields) {
    target.draw(h);
  }
}
