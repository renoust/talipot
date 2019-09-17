/**
 *
 * Copyright (C) 2019  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

//=======================================================================
// DataSet implementation
template <typename T>
bool tlp::DataSet::get(const std::string &str, T &value) const {
  for (std::list<std::pair<std::string, tlp::DataType *>>::const_iterator it = data.begin();
       it != data.end(); ++it) {
    const std::pair<std::string, tlp::DataType *> &p = *it;

    if (p.first == str) {
      value = *(static_cast<T *>(p.second->value));
      return true;
    }
  }

  return false;
}

template <typename T>
bool tlp::DataSet::getAndFree(const std::string &str, T &value) {
  for (std::list<std::pair<std::string, tlp::DataType *>>::iterator it = data.begin();
       it != data.end(); ++it) {
    std::pair<std::string, tlp::DataType *> &p = *it;

    if (p.first == str) {
      value = *(static_cast<T *>(p.second->value));
      delete p.second;
      data.erase(it);
      return true;
    }
  }

  return false;
}

template <typename T>
void tlp::DataSet::set(const std::string &key, const T &value) {
  TypedData<T> dtc(new T(value));
  setData(key, &dtc);
}
//=======================================================================
