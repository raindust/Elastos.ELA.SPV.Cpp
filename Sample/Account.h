// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __SPVCLIENT_ACCOUNT_H__
#define __SPVCLIENT_ACCOUNT_H__

#include <iostream>
#include <boost/program_options/variables_map.hpp>

#include "TWallet.h"

namespace Elastos {
    namespace APP {

        class Account {

        public:
            static bool addAccount(boost::shared_ptr<TWallet> twallet, std::string publickey);
            static bool addMultiSignAccount(boost::shared_ptr<TWallet> twallet, const boost::program_options::variables_map& vm, std::string publickey);
            static bool deleteAccount(boost::shared_ptr<TWallet> twallet, std::string address);

        };

    }
}


#endif //SPVCLIENT_ACCOUNT_H
