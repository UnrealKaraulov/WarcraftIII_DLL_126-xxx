<?php

namespace Warcis\ServerNickName;

use XF\Db\Schema\Alter;
use XF\Db\Schema\Create;

class Setup extends \XF\AddOn\AbstractSetup
{
    use \XF\AddOn\StepRunnerInstallTrait;
    use \XF\AddOn\StepRunnerUpgradeTrait;
    use \XF\AddOn\StepRunnerUninstallTrait;

    public function installStep1()
    {
        $this->schemaManager()->alterTable('xf_user', function(Alter $table)
        {
            $table->addColumn('nickname', 'varchar', 50)->setDefault("Nickname");
        });
    }
}