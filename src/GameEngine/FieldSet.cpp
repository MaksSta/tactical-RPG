#include "FieldSet.h"

std::vector<Field*> FieldSet::get()
{
  return field;
}

Field* FieldSet::getLastElement()
{
  return field.back();
}

void FieldSet::reduce_to_n_elements(int N)
{
  // redukcja do ilości elementów większej/równej obecnie - nic nie rób
  if (N >= field.size())
    return;

  std::vector<Field*> new_fields;

  for (int n = 0; n < N; n++)
  {
    new_fields.push_back(field[n]);
  }

  field = new_fields;
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

void FieldSet::draw(sf::RenderTarget &target,
                    sf::RenderStates states) const
{
  for (auto & h : hightlighOnFields) {
    target.draw(h);
  }
}
